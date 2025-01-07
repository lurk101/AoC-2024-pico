#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day18.txt"
};

struct point {
    int x, y;
    bool operator==(const point& p) const { return (y == p.y) && (x == p.x); }
    point operator+(const point& p) const { return {x + p.x, y + p.y}; }
};

static vector<point> bytes;
static const vector<point> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static constexpr point start = {0, 0}, finish = {70, 70};

static auto Part1(int cycles) {
    int dx = finish.x - start.x + 1;
    int dy = finish.y - start.y + 1;
    vector<vector<bool>> grid(dx, vector<bool>(dy, false));
    vector<vector<bool>> visited(dx, vector<bool>(dy, false));
    vector<vector<int>> distance(dx, vector<int>(dy, 0));
    for (int i = 0; i <= cycles; i++) grid[bytes[i].x][bytes[i].y] = true;
    queue<pair<point, int>> q;
    q.push({start, 0});
    visited[start.x][start.y] = true;
    while (q.size()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) return d;
        for (auto dir : dirs) {
            auto [x, y] = cur + dir;
            if (x < start.x || x > finish.x || y < start.y || y > finish.y || visited[x][y] ||
                grid[x][y])
                continue;
            visited[x][y] = true;
            distance[x][y] = d + 1;
            q.push({{x, y}, d + 1});
        }
    }
    if (!visited[finish.x][finish.y]) return -1;
    return distance[finish.x][finish.y];
}

static auto Part2(int start) {
    for (int i = start; i < bytes.size(); i++)
        if (Part1(i) == -1) return to_string(bytes[i].x) + "," + to_string(bytes[i].y);
    return string("not found");
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    for (const auto& line : lines) {
        int x, y;
        sscanf(line.c_str(), "%d,%d", &x, &y);
        bytes.push_back({x, y});
    }
    cout << "Day 18: RAM Run" << endl
         << "Part 1   - " << Part1(1023) << endl
         << "Part 2   - " << Part2(1024) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
