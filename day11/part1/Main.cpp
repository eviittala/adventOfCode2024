#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

bool evenNbrOfDigits(const uint64_t val) {
    return (std::to_string(val).size() % 2) == 0;
}

uint64_t getLeftHalf(const uint64_t val) {
    const std::string str{std::to_string(val)};
    const size_t size = str.size();
    return std::stoll(str.substr(0, size / 2));
}

uint64_t getRightHalf(const uint64_t val) {
    const std::string str{std::to_string(val)};
    const size_t size = str.size();
    return std::stoll(str.substr(size / 2));
}

uint64_t getResult(const std::string& str) {
    auto data = common::parseUintData(str).at(0);

    std::vector<uint64_t> temp;

    for (size_t blinks{}; blinks < 25; ++blinks) {
        for (size_t i{}; i < data.size(); ++i) {
            const uint64_t val{data.at(i)};
            if (val == 0) {
                temp.push_back(val + 1);
            } else if (evenNbrOfDigits(val)) {
                temp.push_back(getLeftHalf(val));
                temp.push_back(getRightHalf(val));
            } else {
                temp.push_back(val * 2024);
            }
        }
        data = std::move(temp);
    }

    return data.size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
