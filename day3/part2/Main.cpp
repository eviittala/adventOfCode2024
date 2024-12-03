#include <cstdint>
#include <iostream>
#include <regex>

#include "common.hpp"

std::vector<std::vector<int32_t>> vectors;

void parseData(const std::string& txt) {
    std::istringstream in(txt);
    std::regex re(R"((\d+))");
    std::smatch match;
    for (std::string line; std::getline(in, line);) {
        std::vector<int32_t> tmp;
        while (std::regex_search(line, match, re)) {
            tmp.push_back(std::stoi(match[1].str().c_str()));
            line = match.suffix();
        }
        vectors.push_back(std::move(tmp));
    }
}

bool checkData(const std::vector<int32_t>& vec) {
    return (std::is_sorted(std::begin(vec), std::end(vec),
                           [](const auto next, const auto first) {
                               if (std::abs(next - first) > 3) return true;
                               if (first == next) return true;
                               return first > next;
                           }) ||
            std::is_sorted(std::begin(vec), std::end(vec),
                           [](const auto next, const auto first) {
                               if (std::abs(next - first) > 3) return true;
                               if (first == next) return true;
                               return next > first;
                           })

    );
}

uint32_t getResult() {
    uint32_t result{};
    for (const auto& vec : vectors) {
        if (checkData(vec)) {
            ++result;
        } else {
            for (size_t i{}; i < vec.size(); ++i) {
                std::vector<int32_t> tmp(std::begin(vec), std::end(vec));
                tmp.erase(std::begin(tmp) + i);
                if (checkData(tmp)) {
                    ++result;
                    break;
                }
            }
        }
    }
    return result;
}

int main(int args, char* argv[]) {
    auto str = common::readFile(common::getInputFileDir(__FILE__));
    parseData(str);
    std::cout << "Result: " << getResult() << std::endl;
    return 0;
}