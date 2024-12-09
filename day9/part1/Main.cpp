#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "common.hpp"

// void swap(char c1, char

uint64_t getCheckSum(const std::string& str) {
    uint64_t checkSum{};
    const auto size = str.find_first_of(".");

    for (uint64_t i{}; i < size; ++i) {
        const char c = str[i];
        const uint64_t sum = i * (c - '0');
        checkSum += sum;
    }

    return checkSum;
}

std::string getBlocks(const std::string& str) {
    std::string ret;
    uint64_t c{};
    for (size_t i{}; i < str.size(); ++i) {
        if (0 < std::isdigit(str[i])) {
            const size_t count = str[i] - '0';
            if (i % 2 == 0) {
                const char y = '0' + c++;
                try {
                    ret.append(count, y);
                } catch (const std::length_error&) {
                    std::cout << "length error: " << str[i] << std::endl;
                }
            } else {
                try {
                    ret.append(count, '.');
                } catch (const std::length_error&) {
                    std::cout << "length error2 : " << str[i] << std::endl;
                }
            }
        }
    }
    return ret;
}

uint64_t getResult(const std::string& str) {
    auto blocks = getBlocks(str);
    // std::cout << blocks << std::endl;

    while (true) {
        const auto pos1 = blocks.find_first_of(".");
        const auto pos2 = blocks.find_last_not_of(".\n");
        if (pos1 < pos2) {
            std::swap(blocks[pos1], blocks[pos2]);
        } else
            break;
    }
    // std::cout << str << std::endl;
    return getCheckSum(blocks);
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    // std::cout << str << std::endl;
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
