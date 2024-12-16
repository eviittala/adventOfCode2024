#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "common.hpp"

std::vector<std::string> parseWarehouse(const std::string& str) {
    std::regex re(R"((#.*#))");
    std::vector<std::string> ret;
    std::smatch sm;
    std::string tmp = str;

    while (std::regex_search(tmp, sm, re)) {
        ret.push_back(sm[1].str());
        tmp = sm.suffix();
    }

    return ret;
}

std::string parseInstructions(const std::string& str) {
    std::regex re(R"(([<v>^]))");
    std::string ret;
    std::smatch sm;
    std::string tmp = str;

    while (std::regex_search(tmp, sm, re)) {
        ret += sm[1].str();
        tmp = sm.suffix();
    }

    return ret;
}

std::pair<size_t, size_t> getPos(const std::vector<std::string>& vec) {
    for (size_t y{}; y < vec.size(); ++y) {
        const auto str = vec.at(y);
        const size_t pos = str.find('@');
        if (pos != std::string::npos) {
            return {pos, y};
        }
    }
    abort();
}

void makeMove(std::vector<std::string>& vec, const char dir) {
    const auto curPos = getPos(vec);
    // std::cout << vec.at(curPos.second)[curPos.first] << "(" << curPos.first
    //           << ", " << curPos.second << ")" << " dir: " << dir <<
    //           std::endl;

    switch (dir) {
        case '^':  // up
        {
            for (size_t newPos = curPos.second - 1; 0 < newPos; --newPos) {
                if (vec.at(newPos)[curPos.first] == '#') break;
                if (vec.at(newPos)[curPos.first] == '.') {
                    for (size_t i{newPos}; i != curPos.second; ++i) {
                        std::swap(vec.at(i)[curPos.first],
                                  vec.at(i + 1)[curPos.first]);
                    }
                    break;
                }
            }
            break;
        }

        case '>':  // right
        {
            const size_t size = vec.at(0).size();
            for (size_t newPos = curPos.first + 1; newPos < size; ++newPos) {
                if (vec.at(curPos.second)[newPos] == '#') break;
                if (vec.at(curPos.second)[newPos] == '.') {
                    for (size_t i{newPos}; curPos.first < i; --i) {
                        std::swap(vec.at(curPos.second)[i],
                                  vec.at(curPos.second)[i - 1]);
                    }
                    break;
                }
            }
            break;
        }
        case 'v':  // down
        {
            for (size_t newPos = curPos.second + 1; 0 < newPos; ++newPos) {
                if (vec.at(newPos)[curPos.first] == '#') break;
                if (vec.at(newPos)[curPos.first] == '.') {
                    for (size_t i{newPos}; i != curPos.second; --i) {
                        std::swap(vec.at(i)[curPos.first],
                                  vec.at(i - 1)[curPos.first]);
                    }
                    break;
                }
            }
            break;
        }
        case '<':  // left
        {
            for (size_t newPos = curPos.first - 1; 0 < newPos; --newPos) {
                if (vec.at(curPos.second)[newPos] == '#') break;
                if (vec.at(curPos.second)[newPos] == '.') {
                    for (size_t i{newPos}; i < curPos.first; ++i) {
                        std::swap(vec.at(curPos.second)[i],
                                  vec.at(curPos.second)[i + 1]);
                    }
                    break;
                }
            }
            break;
        }
        default:
            abort();
    }
}

uint64_t calculateGpsCoornidate(const std::vector<std::string>& vec) {
    uint64_t res{};
    for (size_t y{1}; y < vec.size(); ++y) {
        const auto& str = vec.at(y);
        for (size_t x{}; x < str.size(); ++x) {
            if (str[x] == 'O') {
                res += 100 * y + x;
            }
        }
    }
    return res;
}

uint64_t getResult(const std::string& str) {
    auto wareHouse = parseWarehouse(str);
    const auto instructions = parseInstructions(str);

    // common::printVecStr(wareHouse);
    // std::cout << instructions << std::endl;

    for (size_t i{}; i < instructions.size(); ++i) {
        makeMove(wareHouse, instructions.at(i));
        // common::printVecStr(wareHouse);
    }

    return calculateGpsCoornidate(wareHouse);
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
