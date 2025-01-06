#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> grid = {
#include "day12.txt"
};

static set<pair<int, int>> visited;

static void Visit(int y, int x, map<pair<int, int>, int>& spot, int a, char ch) {
    if ((y < 0 || y >= grid.size() || x < 0 || x >= grid[0].size()) || visited.contains({y, x}) ||
        (grid[y][x] != ch))
        return;
    visited.insert({y, x});
    spot[{y, x}] = a;
    Visit(y + 1, x, spot, a, ch);
    Visit(y - 1, x, spot, a, ch);
    Visit(y, x + 1, spot, a, ch);
    Visit(y, x - 1, spot, a, ch);
}

static int Corners(int y, int x, map<pair<int, int>, int>& spot) {
    int cur = spot[{y, x}], top = spot[{y - 1, x}], bot = spot[{y + 1, x}], left = spot[{y, x - 1}],
        right = spot[{y, x + 1}], topLeft = spot[{y - 1, x - 1}], topRight = spot[{y - 1, x + 1}],
        botLeft = spot[{y + 1, x - 1}], botRight = spot[{y + 1, x + 1}], s = 0;
    if (cur != top && cur != left) s++;
    if (cur != top && cur != right) s++;
    if (cur != bot && cur != left) s++;
    if (cur != bot && cur != right) s++;
    if (cur == right && cur == bot && cur != botRight) s++;
    if (cur == left && cur == bot && cur != botLeft) s++;
    if (cur == right && cur == top && cur != topRight) s++;
    if (cur == left && cur == top && cur != topLeft) s++;
    return s;
}

static auto Part1() {
    map<pair<int, int>, int> spot;
    map<int, pair<int, int>> sizes;
    int a = 1;
    for (int y = 0; y < grid.size(); y++)
        for (int x = 0; x < grid[0].size(); x++)
            if (!visited.contains({y, x})) Visit(y, x, spot, a++, grid[y][x]);
    for (int y = 0; y < grid.size(); y++)
        for (int x = 0; x < grid[0].size(); x++) {
            sizes[spot[{y, x}]].first++;
            for (int j = 0; j < 4; ++j) {
                int ny = y + "0121"[j] - '1';
                int nx = x + "1210"[j] - '1';
                if (ny < 0 || ny >= grid.size() || nx < 0 || nx >= grid[0].size())
                    sizes[spot[{y, x}]].second++;
                else if (spot[{ny, nx}] != spot[{y, x}])
                    sizes[spot[{y, x}]].second++;
            }
        }
    uint32_t s = 0;
    for (const auto& [_, v] : sizes) s += v.first * v.second;
    return s;
}

static auto Part2() {
    visited.clear();
    map<pair<int, int>, int> spot;
    map<int, pair<int, int>> sizes;
    int a = 1;
    for (int y = 0; y < grid.size(); y++)
        for (int x = 0; x < grid[0].size(); x++)
            if (!visited.contains({y, x})) Visit(y, x, spot, a++, grid[y][x]);
    for (int y = 0; y < grid.size(); y++)
        for (int x = 0; x < grid[0].size(); x++) {
            sizes[spot[{y, x}]].first++;
            sizes[spot[{y, x}]].second += Corners(y, x, spot);
        }
    uint32_t s = 0;
    for (const auto& [_, v] : sizes) s += v.first * v.second;
    return s;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    cout << "Day 12: Garden Groups" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
