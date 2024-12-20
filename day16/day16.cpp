#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> grid = {
#include "day16.txt"
};
static vector<pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
static pair<int, int> start, finish;
static map<tuple<int, int, int>, int> dist;
static map<tuple<int, int, int>, vector<tuple<int, int, int>>> previous;

static uint64_t Part1() {
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>,
                   greater<tuple<int, int, int, int>>>
        q;
    q.push({0, start.first, start.second, 0});
    dist[{start.first, start.second, 0}] = 0;
    while (!q.empty()) {
        auto [dst, row, col, dir] = q.top();
        q.pop();
        if (grid[row][col] == 'E') return dst;
        for (int i = 0; i < 4; i++) {
            int nRow = row + dirs[i].first, nCol = col + dirs[i].second, nDst = dst + 1;
            if (i != dir) nDst += 1000;
            if (nRow < 0 || nCol < 0 || nRow >= grid.size() || nCol >= grid[0].size()) continue;
            if (grid[nRow][nCol] == '#') continue;
            auto cur = make_tuple(nRow, nCol, i);
            if (dist.find(cur) == dist.end() || nDst < dist[cur]) {
                dist[cur] = nDst;
                q.push(make_tuple(nDst, nRow, nCol, i));
                previous[cur].clear();
                previous[cur].push_back({row, col, dir});
            } else if (nDst == dist[cur])
                previous[cur].push_back({row, col, dir});
        }
    }
    return numeric_limits<int>::max();
}

static uint64_t Part2() {
    auto [row, col] = finish;
    set<pair<int, int>> unique;
    queue<tuple<int, int, int>> q;
    q.push({row, col, 0});
    q.push({row, col, 1});
    q.push({row, col, 2});
    q.push({row, col, 3});
    while (!q.empty()) {
        auto [cRow, cCol, cDir] = q.front();
        q.pop();
        unique.insert({cRow, cCol});
        for (const auto& prev : previous[{cRow, cCol, cDir}]) q.push(prev);
    }
    return unique.size();
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    int row = 0;
    for (const auto& line : grid) {
        for (int col = 0; col < line.size(); col++)
            if (line[col] == 'S')
                start = make_pair(row, col);
            else if (line[col] == 'E')
                finish = make_pair(row, col);
        ++row;
    }
    cout << "Day 16: Reindeer Maze" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
