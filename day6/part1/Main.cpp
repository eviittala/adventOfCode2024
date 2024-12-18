#include <cstdint>
#include <iostream>
#include <regex>
#include <set>

#include "Common.hpp"

std::vector<int> makeDirs(const int len) { return {-len, 1, len, -1}; }

uint32_t getResult(const std::string& str) {
    const int len = str.find('\n') + 1;
    const auto dirs = makeDirs(len);
    std::set<int> visit;
    int dir{0};
    int pos = str.find('^');

    while (true) {
        visit.insert(pos);
        const int newPos = pos + dirs.at(dir);

        if (newPos < 0 || newPos >= str.length() || str.at(newPos) == '\n') {
            break;
        }

        if (str.at(newPos) == '.' || str.at(newPos) == '^') {
            pos = newPos;
        } else {
            dir = (dir + 1) % dirs.size();
        }
    }

    return visit.size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
