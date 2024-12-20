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

struct Point {
    bool visited{};
    uint64_t steps{UINT64_MAX};
    std::pair<int, int> dir{};
    std::vector<std::pair<int, int>> pre;
};

std::map<std::pair<int, int>, Point> points;
size_t line_size{};

void printPoints() {
    for (const auto& [pos, point] : points) {
        std::cout << "(" << pos.first << ", " << pos.second << "), ";
        std::cout << point.steps << ", ";
        std::cout << "(" << point.dir.first << ", " << point.dir.second
                  << "), ";
        for (const auto& pre : point.pre) {
            std::cout << "(" << pre.first << ", " << pre.second << "), ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> parseData(const std::string& str) {
    std::regex re(R"((#.*#))");
    std::vector<std::string> ret;
    std::smatch sm;
    std::string tmp = str;

    while (std::regex_search(tmp, sm, re)) {
        ret.push_back(sm[1].str());
        tmp = sm.suffix();
    }

    return ret;
}

std::pair<int, int> getStart(const std::vector<std::string>& vec) {
    for (size_t y{}; y < vec.size(); ++y) {
        const size_t pos = vec.at(y).find('S');
        if (pos != std::string::npos) {
            return {pos, y};
        }
    }
    assert(0);
    return {};
}

std::pair<size_t, size_t> getEnd(const std::vector<std::string>& vec) {
    for (size_t y{}; y < vec.size(); ++y) {
        const size_t pos = vec.at(y).find('E');
        if (pos != std::string::npos) {
            return {pos, y};
        }
    }
    assert(0);
    return {};
}

std::pair<int, int> plus(std::pair<int, int> a, std::pair<int, int> b) {
    return {a.first + b.first, a.second + b.second};
}

std::pair<int, int> getSmallest() {
    uint64_t steps{UINT64_MAX};
    std::pair<int, int> ret;

    for (auto& [pos, p] : points) {
        if (!p.visited && p.steps < steps) {
            steps = p.steps;
            ret = pos;
        }
    }
    return ret;
}

std::set<std::pair<int, int>> tiles;

void getTiles(const std::vector<std::string>& vec,
              const std::pair<int, int> end, const bool firstRun) {
    // auto pos = getEnd(vec);
    // do {
    //     tiles.insert(pos);
    //     pos = points.at(pos).pre;
    // } while (pos.first != INT32_MAX && pos.second != INT32_MAX);

    if (!tiles.insert(end).second && !firstRun) {
        // std::cout << "end: " << end.first << ", " << end.second << std::endl;
        return;
    }
    if (vec.at(end.second)[end.first] == 'S') return;

    for (const auto& pre : points.at(end).pre) {
        getTiles(vec, pre, false);
        // ret.insert(std::begin(pre_tiles), std::end(pre_tiles));
    }
    // return ret;
}

void fillMapAndPrint(const std::vector<std::string>& vec,
                     std::pair<int, int> cur) {
    auto temp = vec;
    // const auto tiles = getTiles(vec, cur);

    for (const auto& tile : tiles) {
        temp.at(tile.second)[tile.first] = 'O';
    }

    common::printVecStr(temp);
}

uint64_t getNextSteps(const std::pair<int, int> c_dir,
                      const std::pair<int, int> n_dir) {
    if (c_dir == n_dir) {
        return 1;
    }
    return 1001;
}

// std::vector<std::pair<int, int>> getDirs(const std::pair<int, int> cur) {
std::vector<std::tuple<uint64_t, std::pair<int, int>>> getDirs(
    const std::pair<int, int> cur) {
    return {{1, {cur.first, cur.second}},
            {1001, {cur.second, -cur.first}},
            {1001, {-cur.second, cur.first}}};
}

void getRoute(const std::vector<std::string>& vec,
              const std::pair<int, int> cur) {
    auto& p = points.at(cur);

    if (!p.visited) {
        for (const auto& [steps, n_dir] : getDirs(p.dir)) {
            auto next = plus(cur, n_dir);
            if (vec.at(next.second)[next.first] == '#') continue;
            auto& n_point = points[next];
            const uint64_t n_steps = p.steps + steps;
            if (n_steps <= n_point.steps) {
                n_point.steps = n_steps;
                n_point.pre.push_back(cur);
                //        if (n_point.pre.size() > 1) {
                //            std::cout << "Size: " << n_point.pre.size() << ":
                //            "
                //                      << next.first << ", " << next.second <<
                //                      std::endl;
                //        }
                n_point.dir = n_dir;
            } else if (n_point.dir != n_dir &&
                       n_point.steps >= (n_steps - 1000)) {
                n_point.pre.push_back(cur);
            }
        }
    }
    p.visited = true;
}

bool areAllPointsVisited() {
    for (const auto& point : points) {
        if (!point.second.visited) {
            return false;
        }
    }
    return true;
}

void findAlternativeRoutes(const std::vector<std::string>& vec) {
    // const auto tiles_t = tiles;
    const auto points_t = points;
    const auto posStart = getStart(vec);
    auto posEnd = getEnd(vec);
    uint64_t c{};

    auto pos = points_t.at(posEnd).pre.at(0);

    while (pos != posStart) {
        printf("%lu:%lu\n", c++, tiles.size());
        points.clear();
        points[posStart].steps = 0;
        points[posStart].dir = std::pair(1, 0);
        while (!areAllPointsVisited()) {
            auto cur = getSmallest();
            if (cur == pos) {
                points[cur].visited = true;
            } else {
                getRoute(vec, cur);
            }
        }
        getTiles(vec, pos, true);
        // if (pos.first == 5 && pos.second == 7) {
        //     printPoints();
        // }
        pos = points_t.at(pos).pre.at(0);
    }

    /*
        pos = points_t.at(pos).pre;
        while (points_t.at(pos).pre.first != INT32_MAX &&
               points_t.at(pos).pre.second != INT32_MAX) {
            for (auto startDir : getDirsStart(points_t.at(pos).dir)) {
                auto cur = pos;
                points.clear();
                points[cur].steps = 0;
                points[cur].dir = startDir;
                while (vec.at(cur.second)[cur.first] != 'E') {
                    auto& p = points.at(cur);
                    if (!p.visited) {
                        p.visited = true;
                        for (auto n_dir : getDirs(p.dir)) {
                            auto next = plus(cur, n_dir);
                            if (vec.at(next.second)[next.first] == '#')
       continue; auto& n_point = points[next]; const uint64_t n_steps =
       p.steps
       + getNextSteps(p.dir, n_dir); if (n_steps <= n_point.steps) {
                                n_point.steps = n_steps;
                                n_point.pre = cur;
                                n_point.dir = n_dir;
                            }
                        }
                    }
                    cur = getSmallest();
                }
            }

            getTiles(vec, {});
            pos = points_t.at(pos).pre;
        }
        */
}

uint64_t getResult(const std::string& str) {
    auto data = parseData(str);
    line_size = data.at(0).size();
    // common::printVecStr(data);
    auto cur = getStart(data);
    auto& point = points[cur];
    point.visited = false;
    // point.pre = std::pair(INT32_MAX, INT32_MAX);
    point.steps = 0;
    // point.tiles = 1;
    point.dir = std::pair(1, 0);

    while (data.at(cur.second)[cur.first] != 'E') {
        // while (!areAllPointsVisited()) {
        getRoute(data, cur);
        cur = getSmallest();
    }
    // std::cout << "Returning: " << data.at(cur.second)[cur.first] <<
    // std::endl;
    cur = getEnd(data);
    std::cout << "steps: " << points.at(cur).steps << std::endl;
    getTiles(data, cur, true);
    // findAlternativeRoutes(data);
    fillMapAndPrint(data, cur);
    return tiles.size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
// 584  // too low
// 585  // too low
