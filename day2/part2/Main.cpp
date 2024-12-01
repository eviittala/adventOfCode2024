#include <cstdint>
#include <filesystem>
#include <iostream>
#include <regex>

#include "common.hpp"

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
        const auto bounds =
            std::equal_range(std::begin(vec2), std::end(vec2), vec1.at(i));
        result += vec1.at(i) * std::distance(bounds.first, bounds.second);
    }
    return result;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult() << std::endl;
    return 0;
}
