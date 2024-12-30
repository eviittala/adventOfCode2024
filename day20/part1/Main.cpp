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
    uint64_t sec{UINT64_MAX};
    friend bool operator<(const Point& l, const Point& r) {
        return l.sec > r.sec;
    }

    Point operator+(const Point& p) { return {this->x + p.x, this->y + p.y}; }
};

bool isValidPoint(const Point& p, const std::vector<std::string>& vec) {
    return 0 <= p.x && p.x < vec.at(0).size() && 0 <= p.y && p.y < vec.size() &&
           vec.at(p.y)[p.x] != '#';
}

std::pair<int, int> getStart(const std::vector<std::string>& vec) {
    for (int y{}; y < vec.size(); ++y) {
        for (int x{}; x < vec.at(0).size(); ++x) {
            if (vec.at(y)[x] == 'S') return {x, y};
        }
    }
    abort();
    return {0, 0};
}

bool isEnd(const Point& p, const std::vector<std::string>& vec) {
    return (vec.at(p.y)[p.x] == 'E');
}

uint64_t findWay(const std::vector<std::string>& vec) {
    std::priority_queue<Point> pq;
    Point first;
    first.sec = 0;
    auto startPos = getStart(vec);
    first.x = startPos.first;
    first.y = startPos.second;
    pq.push(first);
    std::map<std::pair<int, int>, uint64_t> save;

    while (not pq.empty()) {
        auto point = pq.top();
        pq.pop();

        if (isEnd(point, vec)) {
            // std::cout << "Secs: " << point.sec << std::endl;
            return point.sec;
        }

        for (auto [new_sec, new_point] :
             {std::tuple{point.sec + 1, point + Point(0, -1)},
              {std::tuple{point.sec + 1, point + Point(1, 0)}},
              {std::tuple{point.sec + 1, point + Point(0, 1)}},
              {std::tuple{point.sec + 1, point + Point(-1, 0)}}}) {
            new_point.sec = new_sec;
            if (!isValidPoint(new_point, vec)) continue;
            const auto pairXY = std::make_pair(new_point.x, new_point.y);
            if (!save.contains(pairXY) || new_point.sec < save[pairXY]) {
                save[pairXY] = new_point.sec;
                pq.push(new_point);
                // std::cout << pairXY.first << ", " << pairXY.second <<
                // std::endl;
            }
        }
    }
    return 0;
}

std::vector<std::string> getVec(const std::vector<std::string>& vec,
                                const int x, const int y) {
    std::vector<std::string> ret(vec);
    ret.at(y)[x] = '.';
    return ret;
}

uint64_t getResult(const std::string& str) {
    auto data = common::parseLines(str);
    common::printVecStr(data);

    const auto initial_secs = findWay(data);
    std::map<uint64_t, uint64_t> cheats;

    for (int y{1}; y < (data.size() - 1); ++y) {
        for (int x{1}; x < (data.at(0).size() - 1); ++x) {
            if (data.at(y)[x] != '#') continue;
            const auto data_t = getVec(data, x, y);
            const auto secs = findWay(data_t);
            if (secs < initial_secs) {
                const uint64_t saved_secs = initial_secs - secs;
                cheats[saved_secs]++;
            }
        }
    }

    uint64_t ret{};

    for (const auto& [sec, cheat] : cheats) {
        std::cout << "There are " << cheat << " cheats that save " << sec
                  << " picoseconds." << std::endl;
        if (sec >= 100) ret += cheat;
    }

    return ret;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
