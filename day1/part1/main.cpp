#include <cstdint>
#include <iostream>
#include <regex>

#include "Common.hpp"

std::vector<int32_t> vec1;
std::vector<int32_t> vec2;

void parseData(const std::string& txt) {
    std::istringstream in(txt);
    std::regex re(R"((\d+)\s+(\d+))");
    std::smatch match;
    for (std::string line; std::getline(in, line);) {
        if (std::regex_search(line, match, re)) {
            vec1.push_back(std::stoul(match[1].str().c_str()));
            vec2.push_back(std::stoul(match[2].str().c_str()));
        }
    }
}

uint32_t getResult() {
    uint32_t result{};
    for (size_t i{}; i < vec1.size(); ++i) {
        result += std::abs((vec2.at(i) - vec1.at(i)));
    }
    return result;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    parseData(str);
    std::sort(std::begin(vec1), std::end(vec1));
    std::sort(std::begin(vec2), std::end(vec2));
    std::cout << "Result: " << getResult() << std::endl;
    return 0;
}
