#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// Constructor / Destructor
Pmerge::Pmerge() {}
Pmerge::~Pmerge() {}

// Parse input arguments
bool Pmerge::parseInput(int argc, char** argv) {
    if (argc < 2) return false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        std::stringstream ss(arg);
        int num;
        if (!(ss >> num) || num <= 0) return false;
        numbersVector.push_back(num);
        numbersDeque.push_back(num);
    }
    return true;
}

// Getters
const std::vector<int>& Pmerge::getVector() const { return numbersVector; }
const std::deque<int>& Pmerge::getDeque() const { return numbersDeque; }

// Sort vector
void Pmerge::sortVector() { mergeInsertionSortVector(numbersVector); }
void Pmerge::sortDeque() { mergeInsertionSortDeque(numbersDeque); }

// Measure time
double Pmerge::measureTimeVector() {
    std::vector<int> copy = numbersVector;
    clock_t start = clock();
    mergeInsertionSortVector(copy);
    clock_t end = clock();
    numbersVector = copy;
    return double(end - start) / CLOCKS_PER_SEC * 1e6;
}

double Pmerge::measureTimeDeque() {
    std::deque<int> copy = numbersDeque;
    clock_t start = clock();
    mergeInsertionSortDeque(copy);
    clock_t end = clock();
    numbersDeque = copy;
    return double(end - start) / CLOCKS_PER_SEC * 1e6;
}

// Merge-Insertion sort for vector
void Pmerge::mergeInsertionSortVector(std::vector<int>& v) {
    if (v.size() <= 1) return;
    std::vector<int> left, right;
    for (size_t i = 0; i < v.size(); i += 2) {
        left.push_back(v[i]);
        if (i + 1 < v.size()) right.push_back(v[i + 1]);
    }
    mergeInsertionSortVector(left);
    mergeInsertionSortVector(right);

    v.clear();
    size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) v.push_back(left[i++]);
        else v.push_back(right[j++]);
    }
    while (i < left.size()) v.push_back(left[i++]);
    while (j < right.size()) v.push_back(right[j++]);
}

// Merge-Insertion sort for deque
void Pmerge::mergeInsertionSortDeque(std::deque<int>& d) {
    if (d.size() <= 1) return;
    std::deque<int> left, right;
    for (size_t i = 0; i < d.size(); i += 2) {
        left.push_back(d[i]);
        if (i + 1 < d.size()) right.push_back(d[i + 1]);
    }
    mergeInsertionSortDeque(left);
    mergeInsertionSortDeque(right);

    d.clear();
    size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) d.push_back(left[i++]);
        else d.push_back(right[j++]);
    }
    while (i < left.size()) d.push_back(left[i++]);
    while (j < right.size()) d.push_back(right[j++]);
}

