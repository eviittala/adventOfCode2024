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
    int x{};
    int y{};
    uint64_t steps{UINT64_MAX};
    std::pair<int, int> dir{};
    // std::vector<std::pair<int, int>> pre;

    friend bool operator<(const Point& l, const Point& r) {
        return l.steps > r.steps;
    }
};

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

uint64_t getResult(const std::string& str) {
    auto data = parseData(str);
    auto startPos = getStart(data);
    Point point;
    point.x = startPos.first;
    point.y = startPos.second;
    point.steps = 0;
    point.dir = {0, 1};

    std::priority_queue<Point> pq;
    pq.push(point);
    std::set<std::tuple<int, int, int, int>> seen;
    seen.insert({startPos.first, startPos.second, 0, 1});

    while (0 < pq.size()) {
        auto p = pq.top();
        pq.pop();
        seen.insert({p.x, p.y, p.dir.first, p.dir.second});
        if (data.at(p.y)[p.x] == 'E') {
            return p.steps - 1000;
        }
        for (auto [new_steps, nx, ny, ndr, ndc] :
             {std::tuple{p.steps + 1, p.x + p.dir.first, p.y + p.dir.second,
                         p.dir.first, p.dir.second},
              {std::tuple{p.steps + 1000, p.x, p.y, p.dir.second,
                          -p.dir.first}},
              {std::tuple{p.steps + 1000, p.x, p.y, -p.dir.second,
                          p.dir.first}}}) {
            if (data.at(ny)[nx] == '#') continue;
            if (seen.contains({nx, ny, ndr, ndc})) continue;
            Point p_t;
            p_t.x = nx;
            p_t.y = ny;
            p_t.dir = {ndr, ndc};
            p_t.steps = new_steps;
            pq.push(p_t);
        }
    }

    return {};
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
// 584  // too low
// 585  // too low
