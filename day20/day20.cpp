#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day20.txt"
};

struct Point {
    int y, x;
    bool operator==(const Point& p) const { return (y == p.y) && (x == p.x); }
    Point operator+(const Point& p) const { return {y + p.y, x + p.x}; }
};

static vector<string> grid;
static int height, width;
static Point start, finish;
static const vector<Point> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static auto Search(vector<string>& grid, Point& start, Point& finish) {
    vector<vector<int>> dist(height, vector<int>(width, -1));
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    queue<pair<Point, int>> q;
    q.push({start, 0});
    visited[start.y][start.x] = true;
    dist[start.y][start.x] = 0;
    while (!q.empty()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) break;
        for (auto dir : dirs) {
            auto [y, x] = cur + dir;
            if (x < 0 || x >= width || y < 0 || y >= height || visited[y][x] || grid[y][x] == '#')
                continue;
            visited[y][x] = true;
            dist[y][x] = d + 1;
            q.push({{y, x}, d + 1});
        }
    }
    return dist;
}

static inline int Manhattan(Point a, Point b) { return abs(a.y - b.y) + abs(a.x - b.x); }

static int Part(int steps) {
    int s = 0;
    vector<vector<int>> toStart = Search(grid, start, finish),
                        toFinish = Search(grid, finish, start);
    int t = toStart[finish.y][finish.x];
    map<pair<Point, Point>, bool> checked;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == '#') continue;
            for (int newY = max(0, y - steps); newY <= min(height, y + steps); newY++) {
                if (newY < 0 || newY >= height) continue;
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
    string line;
    height = 0;
    ifstream fi("day20.txt");
    while (getline(fi, line)) {
        grid.push_back(line);
        for (int x = 0; x < line.size(); x++)
            if (line[x] == 'S')
                start = {height, x};
            else if (line[x] == 'E')
                finish = {height, x};
        height++;
    }
    width = int(grid[0].size());
    cout << "Day 20: Race Condition" << endl
         << "Part 1   - " << Part(2) << endl
         << "Part 2   - " << Part(20) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
