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
    int p_x{INT32_MAX};
    int p_y{INT32_MAX};
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

std::pair<int, int> getEnd(const std::vector<std::string>& vec) {
    for (int y{}; y < vec.size(); ++y) {
        for (int x{}; x < vec.at(0).size(); ++x) {
            if (vec.at(y)[x] == 'E') return {x, y};
        }
    }
    abort();
    return {0, 0};
}

bool isEnd(const Point& p, const std::vector<std::string>& vec) {
    return (vec.at(p.y)[p.x] == 'E');
}

std::map<std::pair<int, int>, Point> way;

uint64_t findWay(const std::vector<std::string>& vec) {
    std::priority_queue<Point> pq;
    Point first;
    first.sec = 0;
    auto startPos = getStart(vec);
    first.x = startPos.first;
    first.y = startPos.second;
    pq.push(first);

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
            if (!way.contains(pairXY) || new_point.sec < way[pairXY].sec) {
                way[pairXY].sec = new_point.sec;
                way[pairXY].p_x = point.x;
                way[pairXY].p_y = point.y;
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

std::map<std::pair<int, int>, uint64_t> getRoute(
    const std::vector<std::string>& vec) {
    auto pos = getEnd(vec);
    // std::cout << pos.first << ", " << pos.second << std::endl;
    std::vector<std::string> tmp = vec;
    std::map<std::pair<int, int>, uint64_t> route;

    while (way.contains(pos)) {
        route[pos] = way[pos].sec;
        if (vec.at(pos.second)[pos.first] == 'S') break;
        if (vec.at(pos.second)[pos.first] != 'E')
            tmp.at(pos.second)[pos.first] = '0';
        pos = {way[pos].p_x, way[pos].p_y};
    }
    // common::printVecStr(tmp);
    return route;
}

void printMapPair(const std::map<std::pair<int, int>, uint64_t>& map) {
    for (const auto& [pos, sec] : map) {
        std::cout << "(" << pos.first << ", " << pos.second << ") = " << sec
                  << std::endl;
    }
}

uint64_t getResult(const std::string& str) {
    const auto grid = common::parseLines(str);

    const size_t rows = grid.size();
    const size_t cols = grid.at(0).size();

    size_t r{};
    size_t c{};
    for (; r < rows; ++r) {
        bool found(false);
        for (c = 0; c < cols; ++c) {
            if (grid[r][c] == 'S') {
                found = true;
                break;
            }
        }
        if (found) break;
    }

    std::vector<std::vector<int>> dists(rows);
    for (size_t nr{}; nr < rows; ++nr) {
        for (size_t nc{}; nc < cols; ++nc) {
            dists.at(nr).push_back(-1);
        }
    }

    dists[r][c] = 0;

    while (grid.at(r)[c] != 'E') {
        for (auto [nr, nc] : {std::tuple(r + 1, c), std::tuple(r - 1, c),
                              std::tuple(r, c + 1), std::tuple(r, c - 1)}) {
            if (nr < 0 or nc < 0 or nr >= rows or nc >= cols) continue;
            if (grid[nr][nc] == '#') continue;
            if (dists.at(nr).at(nc) != -1) continue;
            dists[nr][nc] = dists[r][c] + 1;
            r = nr;
            c = nc;
        }
    }
    // common::printVecVec(dists);

    uint64_t count{};
    std::set<std::tuple<int, int, int, int>> seen;

    for (r = 0; r < rows; ++r) {
        for (c = 0; c < cols; ++c) {
            if (dists.at(r).at(c) == -1) continue;
            if (grid.at(r)[c] == '#') continue;
            for (int rad{2}; rad < 21; ++rad) {
                for (int dr{}; dr < (rad + 1); ++dr) {
                    const int dc = rad - dr;
                    for (auto [nr, nc] : {std::tuple(r + dr, c + dc),
                                          std::tuple(r + dr, c - dc),
                                          std::tuple(r - dr, c + dc),
                                          std::tuple(r - dr, c - dc)}) {
                        if (nr < 0 or nc < 0 or nr >= rows or nc >= cols)
                            continue;
                        if (grid.at(nr)[nc] == '#') continue;
                        if (dists.at(nr).at(nc) == -1) continue;
                        if (dists[r][c] > dists[nr][nc] &&
                            dists[r][c] - dists[nr][nc] >= 100 + rad) {
                            const auto success = seen.insert({r, c, nr, nc});
                            if (success.second) ++count;
                        }
                    }
                }
            }
        }
    }

    return count;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
