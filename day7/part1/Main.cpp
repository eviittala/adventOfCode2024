#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "common.hpp"

uint64_t calculate(const uint64_t var1, const uint64_t var2, const char c) {
    if (c == '+') return var1 + var2;
    return var1 * var2;
}

bool isValidEquation(const std::vector<uint64_t> vec) {
    const uint64_t sum{vec.at(0)};

    if (sum ==
        (uint64_t)std::accumulate(std::begin(vec) + 1, std::end(vec), 0)) {
        return true;
    }

    if (sum == (uint64_t)std::accumulate(std::begin(vec) + 1, std::end(vec), 1,
                                         std::multiplies<uint64_t>())) {
        return true;
    }

    const size_t sizeOfNbrs = std::distance(std::begin(vec) + 1, std::end(vec));
    std::vector<char> equation;
    for (size_t i{}; i < (sizeOfNbrs - 1); ++i) {
        equation.push_back('*');
    }
    for (size_t i{}; i < (sizeOfNbrs - 1); ++i) {
        equation.push_back('+');
    }

    std::sort(std::begin(equation), std::end(equation));
    do {
        uint64_t tSum{*(std::begin(vec) + 1)};

        for (size_t i{}; i < (sizeOfNbrs - 1); ++i) {
            const uint64_t var1 = *(std::begin(vec) + 1 + i + 1);
            tSum = calculate(tSum, var1, equation.at(i));
        }

        if (sum == tSum) return true;
    } while (std::next_permutation(std::begin(equation), std::end(equation)));

    return false;
}

uint64_t getResult(const std::string& str) {
    uint64_t res{};
    const auto vecs = common::parseUintData(str);

    for (const auto& vec : vecs) {
        if (isValidEquation(vec)) {
            res += vec.at(0);
        }
    }
    return res;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
