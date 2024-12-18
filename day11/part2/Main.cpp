#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

namespace {
constexpr uint64_t nbrOfBlinks{75};
std::map<std::pair<uint64_t, uint64_t>, uint64_t> cache;
}  // namespace

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

uint64_t getNbrOfStones(const uint64_t val, const uint64_t idx) {
    const std::pair key(val, idx);

    if (idx == nbrOfBlinks) {
        return 1;
    }

    if (cache.contains(key)) {
        return cache.at(key);
    }

    uint64_t ret{};
    if (val == 0) {
        ret += getNbrOfStones(val + 1, idx + 1);
    } else if (evenNbrOfDigits(val)) {
        ret += getNbrOfStones(getLeftHalf(val), idx + 1);
        ret += getNbrOfStones(getRightHalf(val), idx + 1);
    } else {
        ret += getNbrOfStones(val * 2024, idx + 1);
    }

    cache[key] = ret;
    return ret;
}

uint64_t getResult(const std::string& str) {
    auto data = common::parseUintData(str).at(0);

    uint64_t stones{};

    for (size_t i{}; i < data.size(); ++i) {
        const uint64_t val{data.at(i)};
        stones += getNbrOfStones(val, 0);
    }

    return stones;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
