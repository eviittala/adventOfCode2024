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
    std::map<std::tuple<int, int, int, int>, uint64_t> lowest_steps;
    std::map<std::tuple<int, int, int, int>, std::vector<Point>> backtrace;
    uint64_t best_steps{UINT64_MAX};
    std::set<std::tuple<int, int, int, int>> end_states;

    while (0 < pq.size()) {
        auto p = pq.top();
        pq.pop();
        if (lowest_steps.contains({p.x, p.y, p.dir.first, p.dir.second})) {
            if (lowest_steps.at({p.x, p.y, p.dir.first, p.dir.second}) <
                p.steps)
                continue;
        }

        if (data.at(p.y)[p.x] == 'E') {
            if (p.steps > best_steps) break;
            best_steps = p.steps;
            end_states.insert({p.x, p.y, p.dir.first, p.dir.second});
        }
        for (auto [new_steps, nx, ny, ndr, ndc] :
             {std::tuple{p.steps + 1, p.x + p.dir.first, p.y + p.dir.second,
                         p.dir.first, p.dir.second},
              {std::tuple{p.steps + 1000, p.x, p.y, p.dir.second,
                          -p.dir.first}},
              {std::tuple{p.steps + 1000, p.x, p.y, -p.dir.second,
                          p.dir.first}}}) {
            if (data.at(ny)[nx] == '#') continue;
            uint64_t lowest{UINT64_MAX};
            if (lowest_steps.contains({nx, ny, ndr, ndc})) {
                lowest = lowest_steps.at({nx, ny, ndr, ndc});
            }

            if (new_steps > lowest) continue;
            if (new_steps < lowest) {
                backtrace[{nx, ny, ndr, ndc}] = {};
                lowest_steps[{nx, ny, ndr, ndc}] = new_steps;
            }
            backtrace[{nx, ny, ndr, ndc}].push_back(
                Point{p.x, p.y, 0, {p.dir.first, p.dir.second}});

            Point p_t;
            p_t.x = nx;
            p_t.y = ny;
            p_t.dir = {ndr, ndc};
            p_t.steps = new_steps;
            pq.push(p_t);
        }
    }
    std::deque<std::tuple<int, int, int, int>> states{std::begin(end_states),
                                                      std::end(end_states)};
    std::set<std::tuple<int, int, int, int>> seen{std::begin(end_states),
                                                  std::end(end_states)};

    while (0 < states.size()) {
        auto key = states.front();
        states.pop_front();
        if (backtrace.contains(key)) {
            for (auto last : backtrace.at(key)) {
                if (seen.contains(
                        {last.x, last.y, last.dir.first, last.dir.second}))
                    continue;
                seen.insert({last.x, last.y, last.dir.first, last.dir.second});
                states.push_back(
                    {last.x, last.y, last.dir.first, last.dir.second});
            }
        }
    }

    std::set<std::tuple<int, int>> seen2;

    for (const auto& [x, y, ndr, ndc] : seen) {
        seen2.insert({x, y});
    }

    return seen2.size();
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
