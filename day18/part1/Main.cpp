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

std::vector<uint64_t> parseProgram(const std::string& str) {
    std::regex re(R"(Program:\s(.*))");
    std::smatch sm;
    std::vector<uint64_t> ret;

    if (std::regex_search(str, sm, re)) {
        // std::cout << sm[1].str() << std::endl;
        std::string temp = sm[1].str();
        while (std::regex_search(temp, sm, std::regex(R"((\d+))"))) {
            ret.push_back(std::stoll(sm[1].str()));
            temp = sm.suffix();
        }
    }

    return ret;
}

uint64_t getResult(const std::string& str) { return {}; }

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
