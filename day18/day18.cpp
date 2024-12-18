#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <pico/stdlib.h>

using namespace std;
using namespace chrono;

static vector<pair<int, int>> bytes;

static const pair<int, int> start = {0, 0};
static const pair<int, int> finish = {70, 70};
static const vector<pair<int, int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static int Part1(int cycle) {
    vector<vector<int>> grid(71, vector<int>(71, 0));
    vector<vector<bool>> visited(71, vector<bool>(71, false));
    vector<vector<int>> dist(71, vector<int>(71, 0));
    for (int i = 0; i <= cycle; i++) grid[bytes[i].first][bytes[i].second] = 1;
    queue<pair<pair<int, int>, int>> q;
    q.push({start, 0});
    visited[start.first][start.second] = true;
    while (!q.empty()) {
        auto [cur, d] = q.front();
        q.pop();
        if (cur == finish) return d;
        for (auto dir : dirs) {
            int x = cur.first + dir.first;
            int y = cur.second + dir.second;
            if (x < 0 || x > 70 || y < 0 || y > 70 || visited[x][y] || grid[x][y]) continue;
            visited[x][y] = true;
            dist[x][y] = d + 1;
            q.push({{x, y}, d + 1});
        }
    }
    if (!visited[finish.first][finish.second]) return -1;
    return dist[finish.first][finish.second];
}

static string Part2() {
    int ans = 0;
    for (int i = 1024; i < bytes.size(); i++)
        if (Part1(i) == -1) return to_string(bytes[i].first) + "," + to_string(bytes[i].second);
    return "not found";
}

static const vector<string> lines = {
#include "day18.txt"
};

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
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
