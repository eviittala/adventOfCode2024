#include <cstdint>
#include <iostream>
#include <regex>

#include "common.hpp"

std::vector<int> makeDirs(const int len) {
    return {-len, -len + 1, 1, len + 1, len, len - 1, -1, -len - 1};
}

uint32_t countWords(const int idx, const std::string& str,
                    const std::vector<int>& dirs) {
    const std::string word = "MAS";
    uint32_t counts{};
    for (const auto dir : dirs) {
        uint32_t hits{};
        int i{idx + dir};
        for (const auto c : word) {
            if (i < 0 || i >= str.length() || c != str.at(i)) {
                break;
            }
            if (++hits == word.length()) {
                ++counts;
            }
            i += dir;
        }
    }
    return counts;
}

uint32_t getResult(const std::string& str) {
    uint32_t res{};
    const int len = str.find('\n') + 1;
    const auto dirs = makeDirs(len);

    for (size_t i{}; i < str.length(); ++i) {
        if (str[i] == 'X') {
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
