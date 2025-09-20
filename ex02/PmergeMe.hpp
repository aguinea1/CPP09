#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>

class Pmerge {
private:
    std::vector<int> numbersVector;
    std::deque<int> numbersDeque;

    // Funciones de ordenamiento privadas
    void mergeInsertionSortVector(std::vector<int>& v);
    void mergeInsertionSortDeque(std::deque<int>& d);

public:
    Pmerge();
    ~Pmerge();

    // Funciones públicas
    bool parseInput(int argc, char** argv); // retorna false si hay error
    void sortVector();
    void sortDeque();
    double measureTimeVector();
    double measureTimeDeque();
    const std::vector<int>& getVector() const;
    const std::deque<int>& getDeque() const;
};

#endif

