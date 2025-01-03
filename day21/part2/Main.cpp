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
