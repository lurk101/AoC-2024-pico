#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day06.txt"
};

static int height, width;
static vector<string> grid;

struct point {
    int y, x;

    bool operator<(const point& p) const {
        if (y == p.y) return x < p.x;
        return y < p.y;
    }

    point operator+(const point& p) const { return {y + p.y, x + p.x}; }

    bool inside() const { return y >= 0 && y < height && x >= 0 && x < width; }
};

static point start;
static vector<point> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
static set<point> visited;

static auto isLoop(vector<string>& grid) {
    int dir = 0;
    set<tuple<int, int, int>> visited;
    point cur = start;
    while (cur.inside()) {
        if (visited.contains({cur.y, cur.x, dir})) return true;
        visited.insert({cur.y, cur.x, dir});
        auto nxt = cur + dirs[dir];
        if (!nxt.inside()) return false;
        if (grid[nxt.y][nxt.x] == '#')
            dir = (dir + 1) % 4;
        else
            cur = cur + dirs[dir];
    }
    return false;
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    ifstream fi("day06.txt");
    string line;
    for (int y = 0; getline(fi, line); y++) {
        grid.push_back(line);
        for (int x = 0; x < line.size(); x++)
            if (line[x] == '^') start = {y, x};
    }
    height = grid.size();
    width = grid[0].size();
    point cur = start;
    int dir = 0;
    while (cur.inside()) {
        visited.insert(cur);
        auto nxt = cur + dirs[dir];
        if (!nxt.inside()) break;
        if (grid[nxt.y][nxt.x] == '#') dir = (dir + 1) & 3;
        cur = cur + dirs[dir];
    }
    int p1 = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if ((grid[i][j] == '.' || grid[i][j] == '^') && visited.contains({i, j})) p1++;
    int p2 = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == '^' || grid[i][j] == '#') continue;
            if (!visited.contains({i, j})) continue;
            grid[i][j] = '#';
            if (isLoop(grid)) p2++;
            grid[i][j] = '.';
        }
    cout << "Day 6: Guard Gallivant" << endl
         << "Part 1   - " << p1 << endl
         << "Part 2   - " << p2 << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
