#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day16.txt"
};

struct point {
    int y, x;
    bool operator<(const point& p) const {
        if (y == p.y) return x < p.x;
        return y < p.y;
    }
};

vector<string> grid;
static vector<point> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
static point start, finish;
static map<tuple<int, int, int>, vector<tuple<int, int, int>>> previous;

static uint64_t Part1() {
    map<tuple<int, int, int>, int> dist;
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>,
                   greater<tuple<int, int, int, int>>>
        q;
    q.push({0, start.y, start.x, 0});
    dist[{start.y, start.x, 0}] = 0;
    while (!q.empty()) {
        auto [dst, y, x, dir] = q.top();
        q.pop();
        if (grid[y][x] == 'E') return dst;
        for (int i = 0; i < 4; i++) {
            int nY = y + dirs[i].y, nX = x + dirs[i].x, nDst = dst + 1;
            if (i != dir) nDst += 1000;
            if (nY < 0 || nX < 0 || nY >= grid.size() || nX >= grid[0].size()) continue;
            if (grid[nY][nX] == '#') continue;
            auto cur = make_tuple(nY, nX, i);
            if (dist.find(cur) == dist.end() || nDst < dist[cur]) {
                dist[cur] = nDst;
                q.push(make_tuple(nDst, nY, nX, i));
                previous[cur].clear();
                previous[cur].push_back({y, x, dir});
            } else if (nDst == dist[cur])
                previous[cur].push_back({y, x, dir});
        }
    }
    return numeric_limits<int>::max();
}

static uint64_t Part2() {
    auto [y, x] = finish;
    set<point> unique;
    queue<tuple<int, int, int>> q;
    q.push({y, x, 0});
    q.push({y, x, 1});
    q.push({y, x, 2});
    q.push({y, x, 3});
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
    ifstream fi("day16.txt");
    string line;
    int y = 0;
    while (getline(fi, line)) {
        grid.push_back(line);
        for (int x = 0; x < line.size(); x++)
            if (line[x] == 'S')
                start = {y, x};
            else if (line[x] == 'E')
                finish = {y, x};
        ++y;
    }
    cout << "Day 16: Reindeer Maze" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
