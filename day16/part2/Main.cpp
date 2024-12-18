#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

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

bool canMove(const std::vector<std::string>& vec, const char dir,
             const std::pair<size_t, size_t> pos) {
    if (vec.at(pos.second)[pos.first] == '.') return true;
    if (vec.at(pos.second)[pos.first] == '#') return false;
    if (dir == '^') {
        const size_t pos1_x =
            vec.at(pos.second)[pos.first] == '[' ? pos.first : pos.first - 1;
        const std::pair<size_t, size_t> pos1{pos1_x, pos.second - 1};
        const std::pair<size_t, size_t> pos2{pos1_x + 1, pos.second - 1};
        return canMove(vec, dir, pos1) && canMove(vec, dir, pos2);
    } else {
        const size_t pos1_x =
            vec.at(pos.second)[pos.first] == '[' ? pos.first : pos.first - 1;
        const std::pair<size_t, size_t> pos1{pos1_x, pos.second + 1};
        const std::pair<size_t, size_t> pos2{pos1_x + 1, pos.second + 1};
        return canMove(vec, dir, pos1) && canMove(vec, dir, pos2);
    }

    return false;
}

void moveUp(std::vector<std::string>& vec,
            const std::pair<size_t, size_t> pos) {
    if (vec.at(pos.second)[pos.first] == '.') {
        std::swap(vec.at(pos.second)[pos.first],
                  vec.at(pos.second + 1)[pos.first]);
    } else if ((vec.at(pos.second)[pos.first] == '[') ||
               (vec.at(pos.second)[pos.first] == ']')) {
        const size_t pos1_x =
            vec.at(pos.second)[pos.first] == '[' ? pos.first : pos.first - 1;
        const std::pair<size_t, size_t> pos1{pos1_x, pos.second - 1};
        const std::pair<size_t, size_t> pos2{pos1_x + 1, pos.second - 1};
        moveUp(vec, pos1);
        moveUp(vec, pos2);
        std::swap(vec.at(pos.second)[pos.first],
                  vec.at(pos.second + 1)[pos.first]);
    } else
        assert(0);
}

void moveDown(std::vector<std::string>& vec,
              const std::pair<size_t, size_t> pos) {
    if (vec.at(pos.second)[pos.first] == '.') {
        std::swap(vec.at(pos.second)[pos.first],
                  vec.at(pos.second - 1)[pos.first]);
    } else if ((vec.at(pos.second)[pos.first] == '[') ||
               (vec.at(pos.second)[pos.first] == ']')) {
        const size_t pos1_x =
            vec.at(pos.second)[pos.first] == '[' ? pos.first : pos.first - 1;
        const std::pair<size_t, size_t> pos1{pos1_x, pos.second + 1};
        const std::pair<size_t, size_t> pos2{pos1_x + 1, pos.second + 1};
        moveDown(vec, pos1);
        moveDown(vec, pos2);
        std::swap(vec.at(pos.second)[pos.first],
                  vec.at(pos.second - 1)[pos.first]);
    } else
        assert(0);
}

void makeMove(std::vector<std::string>& vec, const char dir) {
    const auto curPos = getPos(vec);
    // std::cout << vec.at(curPos.second)[curPos.first] << "(" << curPos.first
    //           << ", " << curPos.second << ")" << " dir: " << dir <<
    //           std::endl;

    switch (dir) {
        case '^':  // up
        {
            if (canMove(vec, dir, {curPos.first, curPos.second - 1})) {
                moveUp(vec, {curPos.first, curPos.second - 1});
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
            if (canMove(vec, dir, {curPos.first, curPos.second + 1})) {
                moveDown(vec, {curPos.first, curPos.second + 1});
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
        for (size_t x{}; x < str.size() - 1; ++x) {
            if ((str[x] == '[') && (str[x + 1] == ']')) {
                res += 100 * y + x;
            }
        }
    }
    return res;
}

void mod(std::vector<std::string>& vec) {
    for (size_t i{}; i < vec.size(); ++i) {
        auto& str = vec.at(i);
        std::string temp;
        for (size_t j{}; j < str.size(); ++j) {
            if (str[j] == '#')
                temp.append(2, '#');
            else if (str[j] == '.')
                temp.append(2, '.');
            else if (str[j] == 'O')
                temp.append("[]");
            else if (str[j] == '@')
                temp.append("@.");
        }
        str = temp;
    }
}

uint64_t getResult(const std::string& str) {
    auto wareHouse = parseWarehouse(str);
    const auto instructions = parseInstructions(str);

    mod(wareHouse);

    // std::cout << std::endl;
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
