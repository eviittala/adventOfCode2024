#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

struct Pos {
    int r{};
    int c{};
    std::string route;

    friend bool operator<(const Pos& a, const Pos& b) {
        return a.route.size() > b.route.size();
    }
};

constexpr uint8_t numberOfDirectionalRobots{25};

Pos pos0;
Pos posDir[numberOfDirectionalRobots];

const std::vector<std::string> robot0{{"789"}, {"456"}, {"123"}, {"X0A"}};
const std::vector<std::string> robot1{{"X^A"}, {"<v>"}};

Pos move(const Pos pos, const char cmd) {
    switch (cmd) {
        case '<':
            return {pos.r, pos.c - 1};
        case '>':
            return {pos.r, pos.c + 1};
        case '^':
            return {pos.r - 1, pos.c};
        case 'v':
            return {pos.r + 1, pos.c};
    }
    assert(false);
    return {};
}

void move_robot0(const char cmd) {
    if (cmd == 'A') {
        std::cout << robot0.at(pos0.r)[pos0.c] << std::endl;
    } else {
        pos0 = move(pos0, cmd);
    }
}

void move_robot1(const char cmd) {
    if (cmd == 'A') {
        move_robot0(robot1.at(posDir[0].r)[posDir[0].c]);
    } else {
        posDir[0] = move(posDir[0], cmd);
    }
}

void move_robot2(const char cmd) {
    if (cmd == 'A') {
        move_robot1(robot1.at(posDir[1].r)[posDir[1].c]);
    } else {
        posDir[1] = move(posDir[1], cmd);
    }
}

void printPos(const Pos pos) {
    std::cout << "(" << pos.r << ", " << pos.c << ")" << std::endl;
}

bool isValidDir(const char last, const char next) {
    if (last == '<' and next == '>') return false;
    if (last == '>' and next == '<') return false;
    if (last == '^' and next == 'v') return false;
    if (last == 'v' and next == '^') return false;
    return true;
}

std::vector<std::string> findShortest(const char c,
                                      const std::vector<std::string>& robot,
                                      Pos& pos) {
    if (robot.at(pos.r)[pos.c] == c) return {"A"};
    std::vector<std::string> routes;
    Pos first(pos.r, pos.c);
    std::priority_queue<Pos> pq;
    pq.push(first);

    while (not pq.empty()) {
        auto p = pq.top();
        pq.pop();

        if (robot.at(p.r)[p.c] == c) {
            if (0 < routes.size() && p.route.size() > routes.at(0).size())
                break;
            routes.push_back(p.route + "A");
            pos.r = p.r;
            pos.c = p.c;
        }

        for (auto [newD, newP] : {std::tuple('^', Pos(p.r - 1, p.c)),
                                  std::tuple('v', Pos(p.r + 1, p.c)),
                                  std::tuple('<', Pos(p.r, p.c - 1)),
                                  std::tuple('>', Pos(p.r, p.c + 1))}) {
            if (0 <= newP.r && newP.r < robot.size() && 0 <= newP.c &&
                newP.c < robot.at(0).size() &&
                robot.at(newP.r)[newP.c] != 'X') {
                if (0 < p.route.size() && !isValidDir(p.route.back(), newD))
                    continue;
                Pos p_t(newP.r, newP.c);
                p_t.route = p.route + newD;
                pq.push(p_t);
            }
        }
    }
    return routes;
}

void printVec(const std::string str, const std::vector<std::string>& vec) {
    std::cout << str << ": ";
    for (const auto& v : vec) {
        std::cout << v << ", ";
    }
    std::cout << std::endl;
}

std::string getShortest(const std::vector<std::string>& str) {
    auto it = std::min_element(std::begin(str), std::end(str),
                               [](const std::string& a, const std::string& b) {
                                   return a.size() < b.size();
                               });
    return *it;
}

std::map<std::pair<std::string, uint64_t>, uint64_t> mem;

uint64_t getShortestDirectionalRoute(const std::string route,
                                     const uint64_t idx) {
    if (idx == numberOfDirectionalRobots) {
        return route.size();
    }

    if (mem.contains(std::pair(route, idx))) {
        return mem.at(std::pair(route, idx));
    }

    uint64_t ret{};
    for (const auto c : route) {
        const auto shortests = findShortest(c, robot1, posDir[idx]);
        std::vector<uint64_t> nextRoutes;
        for (const auto& shortest : shortests) {
            nextRoutes.push_back(
                getShortestDirectionalRoute(shortest, idx + 1));
        }
        ret += *std::min_element(std::begin(nextRoutes), std::end(nextRoutes));
    }
    mem[std::pair(route, idx)] = ret;
    return ret;
}

uint64_t getResult(const std::string& str) {
    auto data = common::parseLines(str);
    // common::printVecStr(data);

    pos0.r = 3;
    pos0.c = 2;

    for (size_t i{}; i < numberOfDirectionalRobots; ++i) {
        posDir[i].r = 0;
        posDir[i].c = 2;
    }

    std::vector<uint64_t> routes;
    for (const auto& code : data) {
        uint64_t route{};
        for (const char in : code) {
            // std::cout << "Searching: " << in << std::endl;
            auto routes0 = findShortest(in, robot0, pos0);
            // printVec("routes0", routes0);
            std::vector<uint64_t> shortest;
            for (const auto& route0 : routes0) {
                shortest.push_back(getShortestDirectionalRoute(route0, 0));
            }
            route +=
                *std::min_element(std::begin(shortest), std::end(shortest));
        }
        routes.push_back(route);
    }

    for (size_t i{}; i < routes.size(); ++i) {
        std::cout << "Code: " << data.at(i) << " Route size: " << routes.at(i)
                  << std::endl;
    }

    uint64_t res{};
    for (size_t i{}; i < routes.size(); ++i) {
        res += routes.at(i) * std::stoi(data.at(i));
    }

    return res;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
