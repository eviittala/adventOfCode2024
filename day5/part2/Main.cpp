#include <cstdint>
#include <iostream>
#include <regex>

#include "common.hpp"

std::vector<int> makeDirs(const int len) {
    return {-len + 1, len - 1, len + 1, -len - 1};
}

bool isValidX(const std::string& str, const int idx1, const int idx2) {
    if (idx1 < 0 || (size_t)idx1 >= str.length()) {
        return false;
    }
    if (idx2 < 0 || (size_t)idx2 >= str.length()) {
        return false;
    }
    return (str[idx1] == 'M' and str[idx2] == 'S') or
           (str[idx2] == 'M' and str[idx1] == 'S');
}

uint32_t countWords(const int idx, const std::string& str,
                    const std::vector<int>& dirs) {
    uint32_t counts{};

    if (isValidX(str, idx + dirs.at(0), idx + dirs.at(1)) &&
        isValidX(str, idx + dirs.at(2), idx + dirs.at(3))) {
        ++counts;
    }

    return counts;
}

uint32_t getResult(const std::string& str) {
    uint32_t res{};
    const int len = str.find('\n') + 1;
    const auto dirs = makeDirs(len);

    for (size_t i{}; i < str.length(); ++i) {
        if (str[i] == 'A') {
            res += countWords(i, str, dirs);
        }
    }

    return res;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
