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

std::vector<std::vector<std::string>> getWays(const std::string& stripe) {
    size_t pos{};
    pos = stripe.find("brwrr");
    if (pos == 0) {
        return {{{"b"}, {"r"}, {"wr"}, {"r"}}, {{"br", "wr", "r"}}};
    }
    pos = stripe.find("bggr");
    if (pos == 0) {
        return {{{"b"}, {"g"}, {"g"}, {"r"}}};
    }
    pos = stripe.find("gbbr");
    if (pos == 0) {
        return {
            {{"g"}, {"b"}, {"b"}, {"r"}},
            {{"g"}, {"b"}, {"br"}},
            {{"gb"}, {"b"}, {"r"}},
            {{"gb"}, {"br"}},
        };
    }
    pos = stripe.find("rrbgbr");
    if (pos == 0) {
        return {
            {{"r"}, {"r"}, {"b"}, {"g"}, {"b"}, {"r"}},
            {{"r"}, {"r"}, {"b"}, {"g"}, {"br"}},
            {{"r"}, {"r"}, {"b"}, {"gb"}, {"r"}},
            {{"r"}, {"rb"}, {"g"}, {"b"}, {"r"}},
            {{"r"}, {"rb"}, {"g"}, {"br"}},
            {{"r"}, {"rb"}, {"gb"}, {"r"}},
        };
    }
    pos = stripe.find("bwurrg");
    if (pos == 0) {
        return {{{"bwu"}, {"r"}, {"r"}, {"g"}}};
    }
    pos = stripe.find("brgr");
    if (pos == 0) {
        return {{{"b"}, {"r"}, {"g"}, {"r"}}, {{"br", "g", "r"}}};
    }
    return {{stripe}};
}

size_t getSize(const std::vector<std::string>& vec) {
    std::string str;
    for (const auto& i : vec) {
        str += i;
    }
    return str.size();
}

uint64_t countPossibleWays(const std::string stripe) {
    if (stripe.size() == 0) return 0;

    uint64_t ret{};

    auto ways = getWays(stripe);
    for (const auto& way : ways) {
        bool success{false};
        for (const auto& m : way) {
            for (const auto& towel : towels) {
                if (towel == m) {
                    success = true;
                    break;
                }
                //                if (std::equal(std::begin(towel),
                //                std::end(towel),
                //                               std::begin(stripe))) {
                //                    ret +=
                //                    countPossibleWays(stripe.substr(towel.size()));
                //                }
            }
        }
        if (success) ++ret;
    }
    ret += countPossibleWays(stripe.substr(getSize(ways.at(0))));

    return ret;
}

uint64_t getResult(const std::string& str) {
    parseData(str);
    // common::printVecStr(towels);
    // common::printVecStr(stripes);
    uint64_t ret{};

    for (const auto& stripe : stripes) {
        ret += countPossibleWays(stripe);
    }

    return ret;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
