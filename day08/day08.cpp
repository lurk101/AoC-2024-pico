#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;
using namespace chrono;

static int maxCol, maxRow{0};
static map<pair<int, int>, char> ants;

static bool inside(const pair<int, int>& p) {
    return p.first >= 0 && p.first < maxRow && p.second >= 0 && p.second < maxCol;
}

static int nodes() {
    set<pair<int, int>> n;
    for (const auto& [point, kind] : ants) {
        for (const auto& [other, otherKind] : ants) {
            if (point == other || kind != otherKind) continue;
            int dx{other.second - point.second}, dy{other.first - point.first};
            pair<int, int> p{other.first + dy, other.second + dx};
            if (inside(p)) n.insert(p);
        }
    }
    return int(n.size());
}

static int harmonics() {
    set<pair<int, int>> n;
    for (const auto& [point, kind] : ants) {
        for (const auto& [other, otherKind] : ants) {
            if (point == other || kind != otherKind) continue;
            int dx{other.second - point.second}, dy{other.first - point.first};
            for (pair<int, int> p{other.first + dy, other.second + dx}; inside(p);
                 p.first += dy, p.second += dx)
                n.insert(p);
            for (pair<int, int> p{other.first - dy, other.second - dx}; inside(p);
                 p.first -= dy, p.second -= dx)
                n.insert(p);
        }
    }
    return int(n.size());
}

static const vector<string> lines = {
#include "day08.txt"
};

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    for (const auto& line : lines) {
        int col{0};
        for (const auto& c : line) {
            if (c != '.') ants.try_emplace({maxRow, col}, c);
            col++;
        }
        maxRow++;
    }
    maxCol = int(lines[0].size());
    cout << "Day 8: Resonant Collinearity" << endl
         << "part 1   - " << nodes() << endl
         << "part 2   - " << harmonics() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
