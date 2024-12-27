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
    int x{INT32_MAX};
    int y{INT32_MAX};
    uint64_t steps{UINT64_MAX};
    friend bool operator<(const Point& l, const Point& r) {
        return l.steps > r.steps;
    }
};

bool isCorrupted(const Point p, const std::vector<Point>& corruptedPoints) {
    return std::find_if(std::begin(corruptedPoints), std::end(corruptedPoints),
                        [&p](const Point& b) {
                            return b.x == p.x and b.y == p.y;
                        }) != std::end(corruptedPoints);
}

std::vector<std::string> makeGrid(const int max_x, const int max_y,
                                  const std::vector<Point>& corruptedPoints) {
    std::vector<std::string> ret;
    for (int y{}; y < max_y; ++y) {
        std::string line;
        for (int x{}; x < max_x; ++x) {
            line += isCorrupted({x, y}, corruptedPoints) ? '#' : '.';
        }
        ret.push_back(std::move(line));
    }
    return ret;
}

std::vector<Point> parseCorruptedBits(const std::string& str) {
    std::regex re(R"((\d+),(\d+))");
    std::smatch sm;
    std::vector<Point> ret;
    std::string tmp = str;

    while (std::regex_search(tmp, sm, re)) {
        Point point;
        point.x = std::stoi(sm[1].str());
        point.y = std::stoi(sm[2].str());
        ret.push_back(std::move(point));
        tmp = sm.suffix();
    }

    return ret;
}

void printC(const std::vector<Point>& vec) {
    for (const auto& p : vec) {
        std::cout << p.x << ", " << p.y << std::endl;
    }
}

std::vector<Point> shrink(const std::vector<Point>& vec, const size_t size) {
    return std::vector<Point>(std::begin(vec), std::begin(vec) + size);
}

bool isValidPoint(const Point& p, const std::vector<std::string>& vec) {
    return 0 <= p.x && p.x < vec.at(0).size() && 0 <= p.y && p.y < vec.size() &&
           vec.at(p.y)[p.x] != '#';
}

bool isOk(const std::vector<Point>& points, const size_t i) {
    auto cPoints = shrink(points, i);

    std::pair<int, int> gridSize{71, 71};
    // std::pair<int, int> gridSize{7, 7};

    auto grid = makeGrid(gridSize.first, gridSize.second, cPoints);
    // printC(cPoints);
    // common::printVecStr(grid);

    std::priority_queue<Point> pq;
    std::map<std::pair<int, int>, Point> safe;
    Point first;
    first.steps = 0;
    first.x = 0;
    first.y = 0;
    pq.push(first);

    while (not pq.empty()) {
        auto point = pq.top();
        pq.pop();

        if (point.x == (gridSize.first - 1) &&
            point.y == (gridSize.second - 1)) {
            // std::cout << "Found " << point.steps << std::endl;
            return true;
        }

        for (auto [new_steps, dir] :
             {std::tuple{point.steps + 1, Point(0, -1)},
              {std::tuple{point.steps + 1, Point(1, 0)}},
              {std::tuple{point.steps + 1, Point(0, 1)}},
              {std::tuple{point.steps + 1, Point(-1, 0)}}}) {
            Point p;
            p.x = point.x + dir.x;
            p.y = point.y + dir.y;
            p.steps = new_steps;
            if (!isValidPoint(p, grid)) continue;
            if (new_steps < safe[{p.x, p.y}].steps) {
                safe[{p.x, p.y}].steps = new_steps;
                // std::cout << "Point: " << p.x << ", " << p.y << ", "
                //           << new_steps << std::endl;
                pq.push(p);
            }
        }
    }
    return false;
}

uint64_t getResult(const std::string& str) {
    auto corruptedPoints = parseCorruptedBits(str);

    for (size_t i{1}; i < corruptedPoints.size(); ++i) {
        if (!isOk(corruptedPoints, i)) {
            std::cout << "The Last nok point: " << corruptedPoints.at(i - 1).x
                      << "," << corruptedPoints.at(i - 1).y << std::endl;
            break;
        }
    }
    return {};
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
