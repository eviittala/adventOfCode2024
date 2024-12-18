#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

struct Token {
    uint64_t a_x{};
    uint64_t a_y{};
    uint64_t b_y{};
    uint64_t b_x{};
    uint64_t p_x{};
    uint64_t p_y{};
};

void printData(const std::vector<Token>& vec) {
    std::cout << "Printing the vec" << std::endl;

    for (const auto& token : vec) {
        std::cout << "A_x: " << token.a_x << ", ";
        std::cout << "A_y: " << token.a_y << ", ";
        std::cout << "B_x: " << token.b_x << ", ";
        std::cout << "B_y: " << token.b_y << ", ";
        std::cout << "P_x: " << token.p_x << ", ";
        std::cout << "P_y: " << token.p_y;
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::vector<Token> parseData(const std::string& str) {
    std::vector<Token> ret;
    std::regex re(
        R"(Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+)\n)");
    std::string temp = str;
    std::smatch sm;

    while (std::regex_search(temp, sm, re)) {
        Token token;
        token.a_x = std::stoull(sm[1].str());
        token.a_y = std::stoull(sm[2].str());
        token.b_x = std::stoull(sm[3].str());
        token.b_y = std::stoull(sm[4].str());
        token.p_x = std::stoull(sm[5].str()) + 10000000000000;
        token.p_y = std::stoull(sm[6].str()) + 10000000000000;
        ret.push_back(token);
        temp = sm.suffix();
    }
    return ret;
}

uint64_t getResult(const std::string& str) {
    uint64_t res{};
    const auto machines = parseData(str);
    // printData(machines);

    for (const auto& machine : machines) {
        uint64_t price{};

        const double a_x = machine.a_x;
        const double a_y = machine.a_y;
        const double b_x = machine.b_x;
        const double b_y = machine.b_y;
        const double p_x = machine.p_x;
        const double p_y = machine.p_y;

        const double press_a =
            (p_x * b_y - p_y * b_x) / (a_x * b_y - a_y * b_x);
        const double press_b = (p_x - a_x * press_a) / b_x;

        if (std::round(press_a) == press_a && std::round(press_b) == press_b) {
            const uint64_t price_t = press_a * 3 + press_b * 1;
            price = price_t;
        }

        res += price;
    }

    return res;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
