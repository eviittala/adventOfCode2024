#include <cstdint>
#include <iostream>
#include <regex>
#include <set>

#include "Common.hpp"

std::vector<int> makeDirs(const int len) { return {-len, 1, len, -1}; }

uint32_t getResult(std::string& str) {
    const int len = str.find('\n') + 1;
    const auto dirs = makeDirs(len);
    std::set<std::pair<int, int>> visit;
    int dir{0};
    int res{};
    const int startPos = str.find('^');

    for (int i{}; i < str.size(); ++i) {
        if (str.at(i) == '\n' || i == startPos || str.at(i) != '.') {
            continue;
        }

        str.at(i) = '0';

        int pos = startPos;
        dir = 0;
        visit.clear();

        while (true) {
            if (!visit.insert({pos, dir}).second) {
                ++res;
                break;
            }
            const int newPos = pos + dirs.at(dir);

            if (newPos < 0 || newPos >= str.length() ||
                str.at(newPos) == '\n') {
                break;
            }

            if (str.at(newPos) == '.' || str.at(newPos) == '^') {
                pos = newPos;
            } else {
                dir = (dir + 1) % dirs.size();
            }
        }

        str.at(i) = '.';
    }

    return res;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
