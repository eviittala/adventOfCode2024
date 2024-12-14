#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "common.hpp"

struct Token {
    int a_x{};
    int a_y{};
    int b_y{};
    int b_x{};
    int p_x{};
    int p_y{};
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
        token.a_x = std::stoi(sm[1].str());
        token.a_y = std::stoi(sm[2].str());
        token.b_x = std::stoi(sm[3].str());
        token.b_y = std::stoi(sm[4].str());
        token.p_x = std::stoi(sm[5].str());
        token.p_y = std::stoi(sm[6].str());
        ret.push_back(token);
        temp = sm.suffix();
    }
    return ret;
}

uint64_t getResult(const std::string& str) {
    uint64_t res{};
    const auto machines = parseData(str);

    for (const auto& machine : machines) {
        uint64_t price{};

        for (int i{1}; i < 101; ++i) {
            const int val_x = machine.a_x * i;
            const int val_y = machine.a_y * i;
            if ((machine.p_x - val_x) % machine.b_x == 0 &&
                (machine.p_y - val_y) % machine.b_y == 0) {
                const int nbr_x_b = (machine.p_x - val_x) / machine.b_x;
                const int nbr_y_b = (machine.p_y - val_y) / machine.b_y;
                if (nbr_x_b == nbr_y_b) {
                    const uint64_t price_t = i * 3 + nbr_x_b * 1;
                    if (price < price_t) {
                        price = price_t;
                    }
                }
            }
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
