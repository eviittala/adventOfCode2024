#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "common.hpp"

uint64_t getCheckSum(const std::vector<uint64_t>& vec) {
    uint64_t checkSum{};

    for (uint64_t i{}; i < vec.size(); ++i) {
        if (vec.at(i) == UINT64_MAX) continue;
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

std::pair<size_t, size_t> findBlock(const std::vector<uint64_t>& blocks,
                                    const uint64_t val, const size_t idx = 0) {
    std::pair<size_t, size_t> ret{UINT64_MAX, UINT64_MAX};

    auto startIt = std::find(std::begin(blocks) + idx, std::end(blocks), val);
    auto endIt =
        std::find_if_not(startIt, std::end(blocks),
                         [val](const uint64_t arg) { return arg == val; });
    return {std::distance(std::begin(blocks), startIt),
            std::distance(std::begin(blocks), endIt)};
    /*
        for (size_t i{idx}; i < blocks.size(); ++i) {
            if (blocks.at(i) == val && ret.first == UINT64_MAX) {
                ret.first = i;
            } else if (blocks.at(i) != val && ret.first != UINT64_MAX &&
                       ret.second == UINT64_MAX) {
                ret.second = i;
                return ret;
            } else if (blocks.at(i) == val && ret.first != UINT64_MAX &&
                       ret.second == UINT64_MAX && (i + 1) == blocks.size()) {
                ret.second = i + 1;
                return ret;
            }
        }
        return ret;
        */
}

bool isValidPair(const std::pair<size_t, size_t>& arg) {
    return arg.first != UINT64_MAX && arg.second != UINT64_MAX;
}

size_t getBlockSize(const std::pair<size_t, size_t>& arg) {
    return arg.second - arg.first;
}

uint64_t getResult(const std::string& str) {
    auto blocks = getBlocks(str);
    int64_t id =
        *std::find_if_not(std::rbegin(blocks), std::rend(blocks),
                          [](const uint64_t arg) { return arg == UINT64_MAX; });

    size_t startIdx{0};
    while (0 < id) {
        auto bounds1 = findBlock(blocks, UINT64_MAX, startIdx);
        auto bounds2 = findBlock(blocks, id);
        if (!isValidPair(bounds1) || !isValidPair(bounds2)) abort();
        if (bounds1.first < bounds2.first &&
            getBlockSize(bounds1) >= getBlockSize(bounds2)) {
            std::swap_ranges(std::begin(blocks) + bounds2.first,
                             std::begin(blocks) + bounds2.second,
                             std::begin(blocks) + bounds1.first);
            startIdx = 0;
        } else {
            startIdx = bounds1.second < bounds2.first ? bounds1.second : 0;
        }
        if (startIdx == 0) --id;
    }
    return getCheckSum(blocks);
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
