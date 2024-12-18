#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

size_t lineLength{};

bool isValidPos(const std::string& str, const int pos) {
    return 0 <= pos && pos < (int)str.size() && str[pos] != '\n';
}

std::set<int> visited;
std::set<int> getPositions(const std::string& str, const int pos,
                           const char c) {
    if (!isValidPos(str, pos) || str[pos] != c) {
        return {};
    }
    if (!visited.insert(pos).second) {
        return {};
    }
    std::set<int> ret{pos};

    for (auto dir : common::getDirections2d(lineLength)) {
        auto positions = getPositions(str, pos + dir, c);
        ret.insert(std::begin(positions), std::end(positions));
    }
    return ret;
}

uint64_t getArea(const std::set<int> positions) { return positions.size(); }

void printSet(const std::set<int>& set) {
    for (const auto i : set) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
}

int getMin(const std::set<int>& positions) {
    int ret{INT32_MAX};
    for (const auto pos : positions) {
        if (pos < ret) ret = pos;
    }
    return ret;
}

int getMax(const std::set<int>& positions) {
    int ret{};
    for (const auto pos : positions) {
        if (pos > ret) ret = pos;
    }
    return ret;
}

uint64_t getPerimeter(const std::string& str, const std::set<int> positions) {
    uint64_t ret{};
    std::vector<std::pair<uint64_t, std::bitset<4>>> pos_t;

    for (auto pos : positions) {
        std::bitset<4> sides{0b1111};

        for (auto dir : common::getDirections2d(lineLength)) {
            const int newPos = pos + dir;
            if (!isValidPos(str, newPos)) continue;
            if (positions.contains(newPos)) {
                if (dir == -(int)lineLength) {
                    sides &= 0b0111;
                } else if (dir == 1) {
                    sides &= 0b1011;
                } else if (dir == (int)lineLength) {
                    sides &= 0b1101;
                } else {
                    sides &= 0b1110;
                }
            }
        }
        pos_t.emplace_back(pos, sides);

        ret += sides.count();
    }
    std::sort(std::begin(pos_t), std::end(pos_t),
              [](const auto a, const auto b) { return a.first < b.first; });
    for (size_t i{1}; i < pos_t.size(); ++i) {
        const auto pre = pos_t.at(i - 1);
        const auto cur = pos_t.at(i);
        if (pre.first + 1 == cur.first && pre.second.test(3) &&
            cur.second.test(3)) {
            ret -= 1;
        }
        if (pre.first + 1 == cur.first && pre.second.test(1) &&
            cur.second.test(1)) {
            ret -= 1;
        }
    }

    for (const auto& posPair : pos_t) {
        const auto& pre = posPair;
        if (pre.second.test(0)) {
            const auto it = std::find_if(
                std::begin(pos_t), std::end(pos_t), [&pre](const auto& a) {
                    return pre.first - lineLength == a.first &&
                           a.second.test(0);
                });
            if (it != std::end(pos_t)) ret -= 1;
        }
        if (pre.second.test(2)) {
            const auto it = std::find_if(
                std::begin(pos_t), std::end(pos_t), [&pre](const auto& a) {
                    return pre.first - lineLength == a.first &&
                           a.second.test(2);
                });
            if (it != std::end(pos_t)) ret -= 1;
        }
    }
    return ret;
}

uint64_t getPrice(const std::string& str, const int pos) {
    if (visited.contains(pos) || !isValidPos(str, pos)) {
        return 0;
    }
    auto positions = getPositions(str, pos, str.at(pos));
    visited.insert(std::begin(positions), std::end(positions));
    if (positions.size() == 1) return 4;
    return getArea(positions) * getPerimeter(str, positions);
}

uint64_t getResult(const std::string& str) {
    uint64_t totalPrice{};
    lineLength = str.find('\n') + 1;

    for (size_t i{}; i < str.size(); ++i) {
        if (!isValidPos(str, i)) continue;
        const auto price = getPrice(str, i);
        totalPrice += price;
    }
    return totalPrice;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
