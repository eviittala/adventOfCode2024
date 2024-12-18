#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>

#include "Common.hpp"

bool isValidNode(const int node, const int pos1, const int pos2,
                 const size_t lineSize) {
    const auto nodeXY = common::getXY(node, lineSize);
    const auto pos1XY = common::getXY(pos1, lineSize);
    const auto pos2XY = common::getXY(pos2, lineSize);
    return common::isCollinear(nodeXY.first, nodeXY.second, pos1XY.first,
                               pos1XY.second, pos2XY.first, pos2XY.second);
}

std::vector<size_t> getAntennas(const std::string& str) {
    std::vector<size_t> ret;
    for (size_t i{}; i < str.size(); ++i) {
        if (str[i] != '.' && str[i] != '\n') {
            ret.push_back(i);
        }
    }
    return ret;
}

bool isAntiNode(const size_t node, const std::vector<size_t>& antennas,
                const std::string& str) {
    const auto lineSize = str.find('\n') + 1;
    for (const auto ant1 : antennas) {
        for (const auto ant2 : antennas) {
            if (ant1 != ant2 && str[ant1] == str[ant2]) {
                if (isValidNode(node, ant1, ant2, lineSize)) {
                    return true;
                }
            }
        }
    }
    return false;
}

uint64_t getResult(const std::string& str) {
    std::set<uint64_t> antiNodes;
    const auto antennas = getAntennas(str);

    for (size_t i{}; i < str.size(); ++i) {
        if (str[i] != '\n' && isAntiNode(i, antennas, str)) {
            antiNodes.insert(i);
        }
    }

    return antiNodes.size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;

    return 0;
}
