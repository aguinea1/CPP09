#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cctype>

RPN::RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other)
        _stack = other._stack;
    return *this;
}

RPN::~RPN() {}

int RPN::evaluate(const std::string& expression) {
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token) {
        if (token.size() == 1 && std::isdigit(token[0])) {
            _stack.push(token[0] - '0');
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");
            int b = _stack.top(); _stack.pop();
            int a = _stack.top(); _stack.pop();
            if (token == "+") _stack.push(a + b);
            else if (token == "-") _stack.push(a - b);
            else if (token == "*") _stack.push(a * b);
            else if (token == "/") {
                if (b == 0) throw std::runtime_error("Error");
                _stack.push(a / b);
            }
        }
        else {
            throw std::runtime_error("Error");
        }
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");
    return _stack.top();
}

