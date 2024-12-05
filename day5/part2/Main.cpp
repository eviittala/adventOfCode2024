#include <cstdint>
#include <iostream>
#include <regex>

#include "common.hpp"

std::vector<std::pair<int, int>> orders;
std::vector<std::vector<int>> updates;

void parseData(const std::string& str) {
    std::regex re(R"((\d+)\|(\d+))");
    std::smatch sm;
    std::string tmp(str);

    while (std::regex_search(tmp, sm, re)) {
        const int var1 = std::stoi(sm[1].str());
        const int var2 = std::stoi(sm[2].str());
        orders.emplace_back(var1, var2);
        tmp = sm.suffix();
    }

    std::regex re2(R"((\d+))");
    std::istringstream iss(tmp);

    for (std::string line; std::getline(iss, line);) {
        std::vector<int> tVec;
        while (std::regex_search(line, sm, re2)) {
            tVec.push_back(std::stoi(sm[1].str()));
            line = sm.suffix();
        }
        if (!tVec.empty()) {
            updates.push_back(tVec);
        }
    }
}

bool checkOrder(const int val1, const int val2) {
    for (auto& [order_num1, order_num2] : orders) {
        if (order_num1 == val2 && order_num2 == val1) {
            return false;
        }
    }
    return true;
}

uint32_t getResult(const std::string& str) {
    uint32_t res{};
    parseData(str);

    for (auto& update : updates) {
        bool success = false;
        for (size_t i{}; i < update.size(); ++i) {
            for (size_t s{}; !success && s < i; ++s) {
                if (!checkOrder(update.at(s), update.at(i))) {
                    success = true;
                }
            }

            for (size_t e{i + 1}; !success && e < update.size(); ++e) {
                if (!checkOrder(update.at(i), update.at(e))) {
                    success = true;
                }
            }
        }

        if (success) {
            std::sort(std::begin(update), std::end(update),
                      [](int a, int b) { return checkOrder(a, b); });
            res += update.at(update.size() / 2);
        }
    }

    return res;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
