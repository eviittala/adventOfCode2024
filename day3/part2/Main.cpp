#include <cstdint>
#include <iostream>
#include <regex>

#include "Common.hpp"

uint32_t getResult(const std::string& str) {
    std::string tmp = str;
    std::string tmp2;
    std::regex re(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::regex skip(R"((.*?)(don't\(\).*?do\(\)))");
    std::smatch sm;
    uint32_t res{};

    while (std::regex_search(tmp, sm, skip)) {
        tmp2 += sm[1].str();
        tmp = sm.suffix();
    }

    if (std::regex_search(tmp, sm, std::regex(R"((.*?)don't\(\))"))) {
        tmp = sm[1].str();
    }

    tmp2 += tmp;

    while (std::regex_search(tmp2, sm, re)) {
        res += std::stoi(sm[1].str()) * std::stoi(sm[2].str());
        tmp2 = sm.suffix();
    }

    return res;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    str = common::removeLineBreaks(str);
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
