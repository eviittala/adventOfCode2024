#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "common.hpp"

uint64_t calculate(const uint64_t var1, const uint64_t var2, const char c) {
    if (c == '+') return var1 + var2;
    if (c == '|')
        return std::stoll(std::to_string(var1) + std::to_string(var2));
    return var1 * var2;
}

char bitsToChar(const uint64_t bits, const size_t idx) {
    switch ((bits >> (idx * 2)) & 0x3) {
        case 0x0:
            return '+';
        case 0x01:
            return '*';
        case 0x02:
            return '|';
    }
    throw std::logic_error("no worries");
}

std::vector<std::vector<char>> getEquations(const uint64_t len) {
    const size_t size = std::pow(4, len);
    std::vector<std::vector<char>> ret(size);
    size_t c{};
    uint64_t bits{0};

    for (size_t i{0}; i < size; ++i) {
        std::vector<char> vec(len);
        try {
            for (size_t j{0}; j < len; ++j) {
                vec.at(j) = bitsToChar(bits, j);
            }
            ret.at(c++) = std::move(vec);
        } catch (const std::logic_error& e) {
        }
        bits += 1;
    }
    ret.resize(c);
    return ret;
}

bool isValidEquation(const std::vector<uint64_t> vec) {
    const uint64_t sum{vec.at(0)};
    const size_t dist = std::distance(std::begin(vec) + 1, std::end(vec));

    for (const auto& equation : getEquations(dist - 1)) {
        uint64_t tSum{*(std::begin(vec) + 1)};

        for (size_t i{2}; i < vec.size(); ++i) {
            const uint64_t var1 = *(std::begin(vec) + i);
            tSum = calculate(tSum, var1, equation.at(i - 2));
            if (sum < tSum) break;
        }

        if (sum == tSum) return true;
    }

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
