#include <pico/stdlib.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<pair<pair<int, int>, pair<int, int>>> bots;

static bool hasEgg(const vector<string>& grid) {
    map<pair<int, int>, bool> visited;
    int maxX = grid[0].size();
    int maxY = grid.size();
    for (int x = 0; x < maxX; x++)
        for (int y = 0; y < maxY; y++)
            if (grid[y][x] == '#' && !visited[{x, y}]) {
                queue<pair<int, int>> q;
                q.push({x, y});
                int count = 0;
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    if (visited[{x, y}]) continue;
                    visited[{x, y}] = true;
                    count++;
                    for (auto [dx, dy] : vector<pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}})
                        if (x + dx < maxX && x + dx >= 0 && y + dy < maxY && y + dy >= 0 &&
                            grid[x + dx][y + dy] == '#')
                            q.push({x + dx, y + dy});
                }
                if (count > 32) return true;
            }
    return false;
}

static uint64_t Part1(int maxX, int maxY, int time) {
    vector<pair<pair<int, int>, pair<int, int>>> r = bots;
    for (int i = 0; i < time; i++)
        for (auto& bot : r) {
            auto& [pos, vel] = bot;
            pos.first += vel.first;
            pos.second += vel.second;
            pos.first += maxX;
            pos.second += maxY;
            pos.first %= maxX;
            pos.second %= maxY;
        }
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (auto bot : r) {
        auto [pos, vel] = bot;
        if (pos.first < maxX / 2 && pos.second < maxY / 2)
            q1++;
        else if (pos.first > maxX / 2 && pos.second < maxY / 2)
            q2++;
        else if (pos.first < maxX / 2 && pos.second > maxY / 2)
            q3++;
        else if (pos.first > maxX / 2 && pos.second > maxY / 2)
            q4++;
    }
    return q1 * q2 * q3 * q4;
}

static int Part2(int maxX, int maxY) {
    vector<pair<pair<int, int>, pair<int, int>>> r = bots;
    for (int i = 1;; i++) {
        for (auto& bot : r) {
            auto& [pos, vel] = bot;
            pos.first += vel.first;
            pos.second += vel.second;
            pos.first += maxX;
            pos.second += maxY;
            pos.first %= maxX;
            pos.second %= maxY;
        }
        vector<string> grid(maxY, string(maxX, '.'));
        for (auto bot : r) {
            auto [pos, vel] = bot;
            grid[pos.second][pos.first] = '#';
        }
        if (hasEgg(grid)) {
            // for (auto l : grid)
            //     cout << l << endl;
            return i;
        }
    }
}

static const vector<string> lines = {
#include "day14.txt"
};

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    ifstream fi("day14.txt");
    for (const auto& line : lines) {
        int x1, y1, x2, y2;
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &x1, &y1, &x2, &y2);
        bots.push_back({{x1, y1}, {x2, y2}});
    }
    cout << "Day 14: Restroom Redoubt" << endl
         << "Part 1   - " << Part1(101, 103, 100) << endl
         << "Part 2   - " << Part2(101, 103) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
