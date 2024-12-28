#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

std::vector<std::string> towels;
std::vector<std::string> stripes;

void parseData(const std::string& str) {
    std::regex re(R"((\w+))");
    std::smatch sm;
    std::istringstream is(str);

    std::string line;
    std::getline(is, line);
    while (std::regex_search(line, sm, re)) {
        towels.push_back(sm[1].str());
        line = sm.suffix();
    }

    while (std::getline(is, line)) {
        if (std::regex_search(line, sm, re)) {
            stripes.push_back(sm[1].str());
        }
    }
}

bool isPossible(const std::string stripe) {
    if (stripe.size() == 0) return true;

    for (const auto& towel : towels) {
        if (std::equal(std::begin(towel), std::end(towel),
                       std::begin(stripe))) {
            if (isPossible(stripe.substr(towel.size()))) {
                return true;
            }
        }
    }

    return false;
}

uint64_t getResult(const std::string& str) {
    parseData(str);
    // common::printVecStr(towels);
    // common::printVecStr(stripes);
    uint64_t ret{};

    for (const auto& stripe : stripes) {
        if (isPossible(stripe)) {
            ++ret;
        }
    }

    return ret;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
