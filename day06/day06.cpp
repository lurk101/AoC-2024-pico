#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> grid;
static pair<int, int> start;
static vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
static set<pair<int, int>> visited;

static auto isLoop(vector<string>& grid) {
    int dir = 0;
    set<tuple<int, int, int>> visited;
    pair<int, int> cur = start;
    while (cur.first >= 0 && cur.first < grid.size() && cur.second >= 0 &&
           cur.second < grid[0].size()) {
        if (visited.contains({cur.first, cur.second, dir})) return true;
        visited.insert({cur.first, cur.second, dir});
        if (cur.first + dirs[dir].first < 0 || cur.first + dirs[dir].first >= grid.size() ||
            cur.second + dirs[dir].second < 0 || cur.second + dirs[dir].second >= grid[0].size())
            return false;
        if (grid[cur.first + dirs[dir].first][cur.second + dirs[dir].second] == '#')
            dir = (dir + 1) % 4;
        else {
            cur.first += dirs[dir].first;
            cur.second += dirs[dir].second;
        }
    }
    return false;
}

static const vector<string> lines = {
#include "day06.txt"
};

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    string line;
    int y = 0;
    for (const auto& line : lines) {
        grid.push_back(line);
        for (int x = 0; x < line.size(); x++)
            if (line[x] == '^') start = {y, x};
        y++;
    }
    pair<int, int> cur = start;
    int dir = 0;
    while (cur.first >= 0 && cur.first < grid.size() && cur.second >= 0 &&
           cur.second < grid[0].size()) {
        visited.insert(cur);
        if (cur.first + dirs[dir].first < 0 || cur.first + dirs[dir].first >= grid.size() ||
            cur.second + dirs[dir].second < 0 || cur.second + dirs[dir].second >= grid[0].size()) {
            break;
        }
        if (grid[cur.first + dirs[dir].first][cur.second + dirs[dir].second] == '#')
            dir = (dir + 1) & 3;
        cur.first += dirs[dir].first;
        cur.second += dirs[dir].second;
    }
    int p1 = 0;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
            if ((grid[i][j] == '.' || grid[i][j] == '^') && visited.contains({i, j})) p1++;
    int p2 = 0;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == '^' || grid[i][j] == '#') continue;
            if (!visited.contains({i, j})) continue;
            grid[i][j] = '#';
            if (isLoop(grid)) p2++;
            grid[i][j] = '.';
        }
    cout << "Day 6: Guard Gallivant" << endl
         << "part 1   - " << p1 << endl
         << "part 2   - " << p2 << endl
         << "Visited time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
