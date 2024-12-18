#include <cstdint>
#include <iostream>
#include <regex>

#include "Common.hpp"

uint32_t getResult(const std::string& str) {
    std::string tmp = str;
    std::regex re(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::smatch sm;
    uint32_t res{};
    while (std::regex_search(tmp, sm, re)) {
        res += std::stoi(sm[1].str()) * std::stoi(sm[2].str());
        tmp = sm.suffix();
    }
    return res;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
