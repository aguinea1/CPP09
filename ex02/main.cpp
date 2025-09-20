#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>

// Función para imprimir secuencia
template <typename Container>
void printSequence(const Container& c) {
    for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it) {
        std::cout << *it;
        if (it + 1 != c.end()) std::cout << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    Pmerge sorter;

    if (!sorter.parseInput(argc, argv)) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::cout << "Before: ";
    printSequence(sorter.getVector());

    double timeVector = sorter.measureTimeVector();
    double timeDeque = sorter.measureTimeDeque();

    std::cout << "After: ";
    printSequence(sorter.getVector());

    std::cout << "Time to process a range of " << sorter.getVector().size()
              << " elements with std::vector : " << std::fixed << std::setprecision(5)
              << timeVector << " us" << std::endl;

    std::cout << "Time to process a range of " << sorter.getDeque().size()
              << " elements with std::deque : " << std::fixed << std::setprecision(5)
              << timeDeque << " us" << std::endl;

    return 0;
}

