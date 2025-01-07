#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> grid = {
#include "day20.txt"
};

static int height, width;

struct point {
    int y, x;
    bool operator==(const point& p) const { return (y == p.y) && (x == p.x); }
    point operator+(const point& p) const { return {y + p.y, x + p.x}; }
    inline bool inside() const { return x >= 0 && x < width && y >= 0 && y < height; }
};

static point start, finish;
static const vector<point> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static auto search(vector<string>& grid, point& start, point& finish) {
    vector<vector<int>> dist(height, vector<int>(width, -1));
    queue<pair<point, int>> q;
    q.push({start, 0});
    dist[start.y][start.x] = 0;
    while (!q.empty()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) break;
        for (auto dir : dirs) {
            auto p = cur + dir;
            if ((!p.inside()) || (dist[p.y][p.x] >= 0) || (grid[p.y][p.x] == '#')) continue;
            dist[p.y][p.x] = d + 1;
            q.push({p, d + 1});
        }
    }
    return dist;
}

static inline auto manhattan(const point& a, const point& b) {
    return abs(a.y - b.y) + abs(a.x - b.x);
}

static auto part(int steps) {
    int s = 0;
    vector<vector<int>> toStart = search(grid, start, finish),
                        toFinish = search(grid, finish, start);
    int t = toStart[finish.y][finish.x];
    map<pair<point, point>, bool> checked;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == '#') continue;
            for (int newY = max(0, y - steps); newY <= min(height, y + steps); newY++) {
                if (newY < 0 || newY >= height) continue;
                for (int newX = max(0, x - steps); newX <= min(width, x + steps); newX++) {
                    if ((newX < 0 || newX >= width) || (grid[newY][newX] == '#') ||
                        (manhattan({y, x}, {newY, newX}) > steps) ||
                        (toStart[y][x] < 0 || toFinish[newY][newX] < 0))
                        continue;
                    int cheat =
                        toStart[y][x] + toFinish[newY][newX] + manhattan({y, x}, {newY, newX});
                    if (t - cheat >= 100) s++;
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
    for (const auto& line : grid) {
        for (int x = 0; x < line.size(); x++)
            if (line[x] == 'S')
                start = {height, x};
            else if (line[x] == 'E')
                finish = {height, x};
        height++;
    }
    width = int(grid[0].size());
    cout << "Day 20: Race Condition" << endl
         << "Part 1   - " << part(2) << endl
         << "Part 2   - " << part(20) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
