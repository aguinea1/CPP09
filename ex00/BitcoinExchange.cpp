#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>

// Constructor / Destructor
BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other)
        _db = other._db;
    return *this;
}
BitcoinExchange::~BitcoinExchange() {}

// Trim spaces
std::string BitcoinExchange::trim(const std::string& s) const {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

// Validate date (basic check)
bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    int y, m, d;
    try {
        y = std::atoi(date.substr(0, 4).c_str());
        m = std::atoi(date.substr(5, 2).c_str());
        d = std::atoi(date.substr(8, 2).c_str());
    } catch (...) {
        return false;
    }
    if (m < 1 || m > 12 || d < 1 || d > 31) return false;
    return true;
}

// Validate value
bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const {
    std::stringstream ss(valueStr);
    ss >> value;
    if (ss.fail() || !ss.eof())
        return false;
    if (value < 0) {
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    if (value > 1000) {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    return true;
}

// Load database CSV
void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file) {
        throw std::runtime_error("Error: could not open database file.");
    }

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, rateStr;
        if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr))
            continue;
        double rate = std::atof(rateStr.c_str());
        _db[date] = rate;
    }
}

// Process input file
void BitcoinExchange::processInput(const std::string& filename) const {
    std::ifstream file(filename.c_str());
    if (!file) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, valueStr;

        if (!std::getline(ss, date, '|')) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        date = trim(date);

        if (!std::getline(ss, valueStr)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        valueStr = trim(valueStr);

        // Validate date
        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        // Validate value
        double value;
        if (!isValidValue(valueStr, value))
            continue;

        // Find closest date <= given date
        std::map<std::string, double>::const_iterator it = _db.lower_bound(date);
        if (it == _db.end() || it->first != date) {
            if (it == _db.begin()) {
                std::cerr << "Error: no earlier data available for " << date << std::endl;
                continue;
            }
            --it;
        }

        double result = value * it->second;
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
}
