#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static int higth, width;
static pair<int, int> start, finish;
static const vector<pair<int, int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
static const vector<string> grid = {
#include "day20.txt"
};

static auto Search(const vector<string>& grid, pair<int, int>& start, pair<int, int>& finish) {
    vector<vector<int>> dist(higth, vector<int>(width, -1));
    vector<vector<bool>> visited(higth, vector<bool>(width, false));
    queue<pair<pair<int, int>, int>> q;
    q.push({start, 0});
    visited[start.first][start.second] = true;
    dist[start.first][start.second] = 0;
    while (!q.empty()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) break;
        for (auto dir : dirs) {
            int y = cur.first + dir.first, x = cur.second + dir.second;
            if (x < 0 || x >= width || y < 0 || y >= higth || visited[y][x] || grid[y][x] == '#')
                continue;
            visited[y][x] = true;
            dist[y][x] = d + 1;
            q.push({{y, x}, d + 1});
        }
    }
    return dist;
}

static inline int Manhattan(pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

static int Part(int steps) {
    int s = 0;
    vector<vector<int>> toStart = Search(grid, start, finish),
                        toFinish = Search(grid, finish, start);
    int t = toStart[finish.first][finish.second];
    map<pair<pair<int, int>, pair<int, int>>, bool> checked;
    for (int y = 0; y < higth; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == '#') continue;
            for (int newY = max(0, y - steps); newY <= min(higth, y + steps); newY++) {
                if (newY < 0 || newY >= higth) continue;
                for (int newX = max(0, x - steps); newX <= min(width, x + steps); newX++) {
                    if ((newX < 0 || newX >= width) || (grid[newY][newX] == '#') ||
                        (Manhattan({y, x}, {newY, newX}) > steps) ||
                        (toStart[y][x] < 0 || toFinish[newY][newX] < 0))
                        continue;
                    int cheat =
                        toStart[y][x] + toFinish[newY][newX] + Manhattan({y, x}, {newY, newX});
                    if (t - cheat >= 100) s++;
                }
            }
        }
    }
    return s;
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    higth = 0;
    for (const auto& line : grid) {
        for (int x = 0; x < line.size(); x++)
            if (line[x] == 'S')
                start = {higth, x};
            else if (line[x] == 'E')
                finish = {higth, x};
        higth++;
    }
    width = int(grid[0].size());
    cout << "Day 20: Race Condition" << endl
         << "Part 1   - " << Part(2) << endl
         << "Part 2   - " << Part(20) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
