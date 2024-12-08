#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "common.hpp"

std::pair<int, int> getXY(const int pos, const size_t lineSize) {
    const int x = pos / lineSize;
    const int y = pos % lineSize;
    return {x, y};
}

bool collinear(const int x1, const int y1, const int x2, const int y2,
               const int x3, const int y3) {
    return (y1 - y2) * (x1 - x3) == (y1 - y3) * (x1 - x2);
}

bool isValidNode(const int node, const int pos1, const int pos2,
                 const size_t lineSize) {
    const auto nodeXY = getXY(node, lineSize);
    const auto pos1XY = getXY(pos1, lineSize);
    const auto pos2XY = getXY(pos2, lineSize);
    return collinear(nodeXY.first, nodeXY.second, pos1XY.first, pos1XY.second,
                     pos2XY.first, pos2XY.second);
}

uint64_t getResult(const std::string& str) {
    std::set<uint64_t> antiNodes;
    const auto lineSize = str.find('\n') + 1;
    const auto dirs = common::getDirections(lineSize);

    for (int i{}; i < str.size(); ++i) {
        if (str[i] != '.' && str[i] != '\n') {
            const char c = str[i];

            for (int j{}; j < str.size(); ++j) {
                if (str[j] == c && j != i) {
                    const int len = j - i;
                    const int node = j + len;

                    if (node >= 0 && node < str.size() && str[node] != '\n' &&
                        isValidNode(node, i, j, lineSize)) {
                        antiNodes.insert(node);
                    }
                }
            }
        }
    }

    return antiNodes.size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
