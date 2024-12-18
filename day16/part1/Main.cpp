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
    std::pair<int, int> dir{};
    std::pair<int, int> pre{};
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

void fillMapAndPrint(const std::vector<std::string>& vec,
                     std::pair<int, int> cur) {
    auto temp = vec;

    while (true) {
        auto& p = points.at(cur);
        cur = p.pre;
        std::cout << "cur: " << cur.first << ", " << cur.second << std::endl;
        if (cur.first == INT32_MAX && cur.second == INT32_MAX) break;
        if (temp.at(cur.second)[cur.first] == 'S') break;
        temp.at(cur.second)[cur.first] = '+';
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

std::vector<std::pair<int, int>> getDirs() {
    return {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
}

uint64_t getResult(const std::string& str) {
    auto data = parseData(str);
    line_size = data.at(0).size();
    // common::printVecStr(data);
    auto cur = getStart(data);
    auto& point = points[cur];
    point.visited = false;
    point.pre = std::pair(INT32_MAX, INT32_MAX);
    point.steps = 0;
    point.dir = std::pair(1, 0);

    while (data.at(cur.second)[cur.first] != 'E') {
        // std::cout << "cur: " << cur.first << ", " << cur.second << std::endl;
        auto& p = points.at(cur);

        if (!p.visited) {
            p.visited = true;
            for (auto n_dir : getDirs()) {
                auto next = plus(cur, n_dir);
                if (data.at(next.second)[next.first] == '#') continue;
                const auto c_dir = p.dir;
                auto& n_point = points[next];
                const uint64_t n_steps = p.steps + getNextSteps(c_dir, n_dir);
                if (n_steps < n_point.steps) {
                    n_point.steps = n_steps;
                    n_point.pre = cur;
                    n_point.dir = n_dir;
                }
            }
        }
        cur = getSmallest();
    }
    // std::cout << "Returning: " << data.at(cur.second)[cur.first] <<
    // std::endl; fillMapAndPrint(data, cur);
    return points.at(cur).steps;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
// 130536
