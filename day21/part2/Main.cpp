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

Pos pos0;
Pos pos1;
Pos pos2;

const std::vector<std::string> robot0{{"789"}, {"456"}, {"123"}, {"X0A"}};
const std::vector<std::string> robot1{{"X^A"}, {"<v>"}};
const std::vector<std::string> robot2{{"X^A"}, {"<v>"}};

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
        move_robot0(robot1.at(pos1.r)[pos1.c]);
    } else {
        pos1 = move(pos1, cmd);
    }
}

void move_robot2(const char cmd) {
    if (cmd == 'A') {
        move_robot1(robot2.at(pos2.r)[pos2.c]);
    } else {
        pos2 = move(pos2, cmd);
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

void addRoute(std::vector<std::string>& vec,
              const std::vector<std::string>& src) {
    if (vec.empty()) {
        if (src.empty()) {
            vec.push_back("A");
        } else {
            for (const auto& s : src) {
                vec.push_back(s + 'A');
            }
        }
    } else {
        if (src.empty()) {
            std::for_each(std::begin(vec), std::end(vec),
                          [](std::string& str) { str += "A"; });

        } else {
            const size_t size = vec.size();
            std::cout << "size: " << src.size() << " vec size: " << vec.size()
                      << std::endl;
            for (size_t i{0}; i < (src.size() - 1); ++i) {
                std::copy(std::begin(vec), std::end(vec),
                          std::back_inserter(vec));
            }

            size_t start{};
            for (const auto& s : src) {
                std::for_each(std::begin(vec) + start,
                              std::begin(vec) + start + size,
                              [&s](std::string& str) { str += s + "A"; });
                start += size;
            }
        }
    }
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

uint64_t getResult(const std::string& str) {
    auto data = common::parseLines(str);
    // common::printVecStr(data);
    std::string cmd =
        "<vA<AA>>^AvAA<^A>A"  // 0
        "<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A";

    pos0.r = 3;
    pos0.c = 2;
    pos1.r = 0;
    pos1.c = 2;
    pos2.r = 0;
    pos2.c = 2;

    std::vector<std::string> routes;
    // const std::vector<std::string> codes{
    //     {"029A"}, {"980A"}, {"179A"}, {"456A"}, {"379A"}};
    //  const std::string code = "2";
    for (const auto& code : data) {
        std::string route;
        for (const char in : code) {
            std::cout << "Searching: " << in << std::endl;
            auto routes0 = findShortest(in, robot0, pos0);
            printVec("routes0", routes0);
            std::string shortest;
            Pos pos1_t(pos1);
            for (const auto& route0 : routes0) {
                pos1 = pos1_t;
                std::string temp;
                for (const char c0 : route0) {
                    const auto routes1 = findShortest(c0, robot1, pos1);
                    printVec("routes1", routes1);
                    std::vector<std::string> routes3;
                    Pos pos2_t(pos2);
                    for (const auto& route1 : routes1) {
                        pos2 = pos2_t;
                        std::string routes2_t;
                        for (const char c1 : route1) {
                            const auto routes2 = findShortest(c1, robot2, pos2);
                            printVec("routes2", routes2);
                            routes2_t += getShortest(routes2);
                        }
                        routes3.push_back(routes2_t);
                    }
                    temp += getShortest(routes3);
                }

                if (shortest.empty() or temp.size() < shortest.size()) {
                    shortest = temp;
                }
            }
            route += shortest;
        }
        routes.push_back(route);
    }

    for (size_t i{}; i < routes.size(); ++i) {
        std::cout << "Code: " << data.at(i) << " Route: " << routes.at(i)
                  << " size: " << routes.at(i).size() << std::endl;
    }

    for (const auto& route : routes) {
        pos0.r = 3;
        pos0.c = 2;
        pos1.r = 0;
        pos1.c = 2;
        pos2.r = 0;
        pos2.c = 2;

        uint64_t counter{};
        for (auto c : route) {
            // std::cout << ++counter << ". cmd: " << c << std::endl;
            move_robot2(c);
            // printPos(pos0);
            // printPos(pos1);
            // printPos(pos2);
        }
    }
    std::cout << std::endl;

    uint64_t res{};

    for (size_t i{}; i < routes.size(); ++i) {
        res += routes.at(i).size() * std::stoi(data.at(i));
    }

    return res;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
