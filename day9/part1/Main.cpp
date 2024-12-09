#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "common.hpp"

void printVec(const std::vector<uint64_t>& vec) {
    for (auto& data : vec) {
        if (data == UINT64_MAX)
            std::cout << '.' << ", ";
        else
            std::cout << data << ", ";
    }
    std::cout << std::endl;
}

uint64_t getCheckSum(const std::vector<uint64_t>& vec) {
    uint64_t checkSum{};

    for (uint64_t i{}; i < vec.size(); ++i) {
        if (vec.at(i) == UINT64_MAX) break;
        const uint64_t sum = i * vec.at(i);
        checkSum += sum;
    }

    return checkSum;
}

std::vector<uint64_t> getBlocks(const std::string& str) {
    std::vector<uint64_t> ret;
    uint64_t c{};
    for (size_t i{}; i < str.size(); ++i) {
        if (0 < std::isdigit(str[i])) {
            const size_t count = str[i] - '0';
            if (i % 2 == 0) {
                for (size_t j{}; j < count; ++j) ret.emplace_back(c);
                ++c;
            } else {
                for (size_t j{}; j < count; ++j) ret.emplace_back(UINT64_MAX);
            }
        }
    }
    return ret;
}

uint64_t getResult(const std::string& str) {
    auto blocks = getBlocks(str);

    while (true) {
        auto it1 = std::find(std::begin(blocks), std::end(blocks), UINT64_MAX);
        auto it2 = std::find_if_not(
            std::rbegin(blocks), std::rend(blocks),
            [](const uint64_t arg) { return arg == UINT64_MAX; });
        if (it1 != std::end(blocks) && it2 != std::rend(blocks) &&
            std::distance(it1, std::end(blocks)) >
                std::distance(std::rbegin(blocks), it2)) {
            std::swap(*it1, *it2);
        } else
            break;
    }
    return getCheckSum(blocks);
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
