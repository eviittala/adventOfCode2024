#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "common.hpp"

struct Token {
    int pos_x{};
    int pos_y{};
    int v_x{};
    int v_y{};
};

void printVec(const std::vector<Token>& args) {
    for (auto& arg : args) {
        std::cout << arg.pos_x << ", ";
        std::cout << arg.pos_y << ", ";
        std::cout << arg.v_x << ", ";
        std::cout << arg.v_y << std::endl;
    };
}

std::vector<Token> parseData(const std::string& str) {
    std::vector<Token> ret;
    std::regex re(R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))");
    std::string temp = str;
    std::smatch sm;

    while (std::regex_search(temp, sm, re)) {
        Token token;
        token.pos_x = std::stoi(sm[1].str());
        token.pos_y = std::stoi(sm[2].str());
        token.v_x = std::stoi(sm[3].str());
        token.v_y = std::stoi(sm[4].str());
        ret.push_back(token);
        temp = sm.suffix();
    }
    return ret;
}

std::vector<std::string> makeGrid(const int wide, const int tall) {
    std::vector<std::string> ret;
    for (int i{}; i < tall; ++i) {
        const std::string ret_t(wide, '.');
        ret.push_back(ret_t);
    }
    return ret;
}

void makeMove(Token& token, const int wide, const int tall) {
    int& pos_x = token.pos_x;
    int& pos_y = token.pos_y;
    const int& v_x = token.v_x;
    const int& v_y = token.v_y;

    pos_x = (pos_x + v_x) % wide;
    if (pos_x < 0) pos_x = wide + pos_x;

    pos_y = (pos_y + v_y) % tall;
    if (pos_y < 0) pos_y = tall + pos_y;
}

void addToGrid(std::vector<std::string>& grid, const Token& token) {
    char nbr = '1';
    if (grid.at(token.pos_y)[token.pos_x] != '.') {
        const int orig = (grid.at(token.pos_y)[token.pos_x] - '0');
        if (orig == 9) std::cout << "Orig is 9!!" << std::endl;
        const int nbr_t = orig + 1;
        nbr = '0' + nbr_t;
    }
    grid.at(token.pos_y)[token.pos_x] = nbr;
}

void printGrid(const std::vector<std::string>& grid) {
    for (size_t i{}; i < grid.size(); ++i) {
        std::cout << grid.at(i) << std::endl;
    }
    std::cout << std::endl;
}

uint64_t getResult(const std::string& str) {
    const int wide{101};
    const int tall{103};
    auto data = parseData(str);
    // printVec(data);
    uint64_t moves{1};

    for (; moves < 10000000000; ++moves) {
        std::set<std::pair<int, int>> positions;
        for (auto& d : data) {
            makeMove(d, wide, tall);
            positions.insert({d.pos_x, d.pos_y});
        }
        if (positions.size() == data.size()) break;
    }
    auto grid = makeGrid(wide, tall);
    for (auto& d : data) {
        addToGrid(grid, d);
    }
    printGrid(grid);

    return moves;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
