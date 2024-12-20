#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

typedef vector<int> point;

set<vector<int>> try1(const vector<string>& grid, int x, int y, int prev_val = -1) {
    set<vector<int>> visited;
    if (!(x >= 0 && x < grid.size()) || !(y >= 0 && y < grid[0].size())) return {{-1, -1}};
    if (grid[x][y] - 48 != prev_val + 1) return {{-1, -1}};
    if (prev_val == 8) return {{x, y}};
    set<vector<int>> temp;
    temp = try1(grid, x + 1, y, prev_val + 1);
    visited.insert(temp.begin(), temp.end());
    temp = try1(grid, x - 1, y, prev_val + 1);
    visited.insert(temp.begin(), temp.end());
    temp = try1(grid, x, y + 1, prev_val + 1);
    visited.insert(temp.begin(), temp.end());
    temp = try1(grid, x, y - 1, prev_val + 1);
    visited.insert(temp.begin(), temp.end());
    return visited;
}

void try2(const vector<string>& grid, int x, int y, set<vector<point>>& out, vector<point> current,
          int prev_val = -1) {
    if (!(x >= 0 && x < grid.size()) || !(y >= 0 && y < grid[0].size())) return;
    if (grid[x][y] - 48 != prev_val + 1) return;
    if (prev_val == 8) {
        current.push_back({x, y});
        out.insert(current);
        return;
    }
    current.push_back({x, y});
    try2(grid, x - 1, y, out, current, prev_val + 1);
    try2(grid, x + 1, y, out, current, prev_val + 1);
    try2(grid, x, y - 1, out, current, prev_val + 1);
    try2(grid, x, y + 1, out, current, prev_val + 1);
}

static const vector<string> grid = {
#include "day10.txt"
};

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    int p1{0};
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
            if (grid[i][j] == '0') p1 += int(try1(grid, i, j).size() - 1);
    int p2{0};
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
            if (grid[i][j] == '0') {
                set<vector<point>> temp;
                try2(grid, i, j, temp, {});
                p2 += int(temp.size());
            }
    cout << "Day 10: Hoof It" << endl
         << "part 1   - " << p1 << endl
         << "part 2   - " << p2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
