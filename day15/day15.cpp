#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day15.txt"
};

struct point {
    int y, x;
    point operator+(const point& p) const { return {y + p.y, x + p.x}; }
};

static vector<string> grid, saveGrid;
static string instrs;
static point pos;
static map<char, point> dirs = {{'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};
static map<char, char> oppDir = {{'^', 'v'}, {'v', '^'}, {'<', '>'}, {'>', '<'}};

static void MovBot(char dir, point& pos) {
    point cur = pos + dirs[dir];
    while (grid[cur.y][cur.x] == 'O') cur = cur + dirs[dir];
    if (grid[cur.y][cur.x] == '#') return;
    while (cur.y != pos.y || cur.x != pos.x) {
        point nexPos{cur + dirs[oppDir[dir]]};
        swap(grid[cur.y][cur.x], grid[nexPos.y][nexPos.x]);
        cur = nexPos;
    }
    pos = pos + dirs[dir];
}

static bool ChkBox(char dir, point pos) {
    point cur = pos + dirs[dir];
    if (grid[cur.y][cur.x] == '.') return true;
    if (grid[cur.y][cur.x] == '#') return false;
    if (grid[cur.y][cur.x] == '[')
        return ChkBox(dir, cur) && (dirs[dir].x + 1 == 0 ? true : ChkBox(dir, {cur.y, cur.x + 1}));
    if (grid[cur.y][cur.x] == ']')
        return ChkBox(dir, cur) && (dirs[dir].x - 1 == 0 ? true : ChkBox(dir, {cur.y, cur.x - 1}));
    return false;
}

static void MovBox(char dir, point pos) {
    point cur = pos + dirs[dir];
    if (grid[cur.y][cur.x] == '.') {
        grid[cur.y][cur.x] = grid[pos.y][pos.x];
        grid[pos.y][pos.x] = '.';
    } else if (grid[cur.y][cur.x] == '[') {
        MovBox(dir, cur);
        if (dirs[dir].x == 0) MovBox(dir, {cur.y, cur.x + 1});
        grid[cur.y][cur.x] = grid[pos.y][pos.x];
        grid[pos.y][pos.x] = '.';
    } else if (grid[cur.y][cur.x] == ']') {
        MovBox(dir, cur);
        if (dirs[dir].x == 0) MovBox(dir, {cur.y, cur.x - 1});
        grid[cur.y][cur.x] = grid[pos.y][pos.x];
        grid[pos.y][pos.x] = '.';
    }
}

static uint64_t Part1() {
    saveGrid = grid;
    uint64_t s = 0;
    for (auto instr : instrs) MovBot(instr, pos);
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
            if (grid[i][j] == 'O') s += i * 100 + j;
    return s;
}

static uint64_t Part2() {
    grid.clear();
    for (auto line : saveGrid) {
        string row = "";
        for (auto c : line) {
            if (c == '@')
                row.append("@.");
            else if (c == '#')
                row.append("##");
            else if (c == '.')
                row.append("..");
            else if (c == 'O')
                row.append("[]");
        }
        grid.push_back(row);
    }
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
            if (grid[i][j] == '@') {
                pos = {i, j};
                goto Found;
            }
Found:
    for (auto instr : instrs)
        if (ChkBox(instr, pos)) {
            MovBox(instr, pos);
            pos = pos + dirs[instr];
        }
    uint64_t s = 0;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
            if (grid[i][j] == '[') s += i * 100 + j;
    return s;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    ifstream fi("day15.txt");
    string line;
    int i = 0;
    while (getline(fi, line)) {
        if (line.empty()) break;
        grid.push_back(line);
        for (int j = 0; j < line.size(); j++)
            if (line[j] == '@') pos = {i, j};
        ++i;
    }
    while (getline(fi, line)) instrs += line;
    cout << "Day 15: Warehouse Woes" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
