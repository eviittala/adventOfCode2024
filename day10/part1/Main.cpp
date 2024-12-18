#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

std::unordered_map<int, std::set<int>> save;

std::set<int> checkNode(const std::string& str, const int pos) {
    if (save.contains(pos)) {
        return save[pos];
    }

    const int val = common::toInt(str.at(pos));
    if (val == 9) {
        return {pos};
    }

    std::set<int> ret;
    const auto lineLength = str.find('\n') + 1;

    for (const auto dir : common::getDirections2d(lineLength)) {
        auto newPos = pos + dir;
        if (common::isValidPos(str, newPos) &&
            (val + 1) == common::toInt(str[newPos])) {
            auto result = checkNode(str, newPos);
            ret.insert(std::begin(result), std::end(result));
        }
    }

    save[pos] = {std::begin(ret), std::end(ret)};
    return ret;
}

uint64_t getResult(const std::string& str) {
    uint64_t scores{};

    for (int i{}; i < (int)str.size(); ++i) {
        if (str[i] == '0') {
            auto result = checkNode(str, i);
            scores += result.size();
        }
    }

    return scores;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
