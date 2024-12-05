#include <cstdint>
#include <iostream>
#include <regex>

#include "common.hpp"

uint32_t getResult(const std::string& str) {
    uint32_t res{};
    return res;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
