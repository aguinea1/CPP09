#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <iomanip>
#include <stdexcept>

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
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

// Validate date (YYYY-MM-DD)
bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;

    int y = std::atoi(date.substr(0, 4).c_str());
    int m = std::atoi(date.substr(5, 2).c_str());
    int d = std::atoi(date.substr(8, 2).c_str());

    if (m < 1 || m > 12) return false;

    int max_day[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) max_day[2] = 29;

    return d >= 1 && d <= max_day[m];
}

// Validate value
bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const {
    std::stringstream ss(valueStr);
    ss >> value;
    if (ss.fail() || !ss.eof())
        return false; // conversión inválida, se tratará como "bad input"

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
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string date, rateStr;
        if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr)) continue;

        date = trim(date);
        rateStr = trim(rateStr);

        double rate = std::atof(rateStr.c_str());
        _db[date] = rate;
    }
}

// Find the closest date <= given date
std::map<std::string, double>::const_iterator BitcoinExchange::findRateForDate(const std::string& date) const {
    std::map<std::string, double>::const_iterator it = _db.lower_bound(date);
    if (it == _db.end() || it->first != date) {
        if (it == _db.begin()) return _db.end(); // no earlier data
        --it;
    }
    return it;
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
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string date, valueStr;

        if (!std::getline(ss, date, '|') || !std::getline(ss, valueStr)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        date = trim(date);
        valueStr = trim(valueStr);

        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        double value;
        if (!isValidValue(valueStr, value)) {
            // Solo imprime mensaje específico en isValidValue, nada más
            continue;
        }

        std::map<std::string, double>::const_iterator it = findRateForDate(date);
        if (it == _db.end()) {
            std::cerr << "Error: no earlier data available for " << date << std::endl;
            continue;
        }

        double result = value * it->second;
        std::cout << date << " => " << std::fixed << std::setprecision(2)
                  << value << " = " << result << std::endl;
    }
}

