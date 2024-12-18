#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

struct Point {
    bool visited{};
    uint64_t steps{UINT64_MAX};
    uint64_t tiles{UINT64_MAX};
    std::pair<int, int> dir{};
    std::vector<std::pair<int, int>> pre{};
};

std::map<std::pair<int, int>, Point> points;
size_t line_size{};

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

std::pair<size_t, size_t> getStart(const std::vector<std::string>& vec) {
    for (size_t y{}; y < vec.size(); ++y) {
        const size_t pos = vec.at(y).find('S');
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
    uint64_t steps{INT32_MAX};
    std::pair<int, int> ret;

    for (auto& [pos, p] : points) {
        if (!p.visited && p.steps < steps) {
            steps = p.steps;
            ret = pos;
        }
    }
    return ret;
}

std::set<std::pair<int, int>> getTiles(const std::vector<std::string>& vec,
                                       const std::pair<int, int> cur) {
    auto& p = points.at(cur);
    std::set<std::pair<int, int>> ret{cur};
    if (vec.at(cur.second)[cur.first] == 'S') return ret;

    for (const auto& pre : p.pre) {
        const auto pre_tiles = getTiles(vec, pre);
        ret.insert(std::begin(pre_tiles), std::end(pre_tiles));
    }
    return ret;
}

void fillMapAndPrint(const std::vector<std::string>& vec,
                     std::pair<int, int> cur) {
    auto temp = vec;
    const auto tiles = getTiles(vec, cur);

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

std::vector<std::pair<int, int>> getDirs(const std::pair<int, int> cur) {
    const std::pair<int, int> up{0, -1};
    const std::pair<int, int> right{1, 0};
    const std::pair<int, int> down{0, 1};
    const std::pair<int, int> left{-1, 0};

    if (cur == up) {
        return {left, up, right};
    }
    if (cur == right) {
        return {up, right, down};
    }
    if (cur == down) {
        return {left, down, right};
    }
    if (cur == left) {
        return {left, up, down};
    }
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
    point.tiles = 1;
    point.dir = std::pair(1, 0);

    while (data.at(cur.second)[cur.first] != 'E') {
        auto& p = points.at(cur);

        if (!p.visited) {
            p.visited = true;
            const auto tiles = p.tiles;
            for (auto n_dir : getDirs(p.dir)) {
                auto next = plus(cur, n_dir);
                if (data.at(next.second)[next.first] == '#') continue;
                auto& n_point = points[next];
                const uint64_t n_steps = p.steps + getNextSteps(p.dir, n_dir);
                if (n_steps <= n_point.steps) {
                    n_point.steps = n_steps;
                    n_point.pre.push_back(cur);
                    n_point.dir = n_dir;
                    n_point.tiles = tiles + 1;
                } else if (tiles <= n_point.tiles) {
                    n_point.pre.push_back(cur);
                }
            }
        }
        cur = getSmallest();
    }
    // std::cout << "Returning: " << data.at(cur.second)[cur.first] <<
    // std::endl;
    // fillMapAndPrint(data, cur);
    return getTiles(data, cur).size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
// 130536
