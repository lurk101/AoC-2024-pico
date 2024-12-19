#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace chrono;

struct Point {
    int row, col;
    auto operator<=>(const Point&) const = default;
};

namespace std {
template <>
struct hash<Point> {
    int operator()(const Point& p) const {
        int h1{int(hash<int>{}(p.row))}, h2{int(hash<int>{}(p.col))};
        return h1 ^ (h2 << 1);
    }
};
};

static const vector<string> lines = {
#include "day08.txt"
};

struct Antennas {

    Antennas() {
        for (const auto& line : lines) {
            int col{0};
            for (const auto& c : line) {
                if (c != '.') ants.try_emplace({maxRow, col}, c);
                col++;
            }
            maxRow++;
            maxCol = int(line.size());
        }
    }

    bool inside(const Point& p) {
        return p.row >= 0 && p.row < maxRow && p.col >= 0 && p.col < maxCol;
    }

    int nodes() {
        unordered_set<Point> n;
        for (const auto& [point, kind] : ants) {
            for (const auto& [other, otherKind] : ants) {
                if (point == other || kind != otherKind) continue;
                int dx{other.col - point.col}, dy{other.row - point.row};
                Point p{other.row + dy, other.col + dx};
                if (inside(p)) n.insert(p);
            }
        }
        return int(n.size());
    }

    int harmonics() {
        unordered_set<Point> n;
        for (const auto& [point, kind] : ants) {
            for (const auto& [other, otherKind] : ants) {
                if (point == other || kind != otherKind) continue;
                int dx{other.col - point.col}, dy{other.row - point.row};
                for (Point p{other.row + dy, other.col + dx}; inside(p); p.row += dy, p.col += dx)
                    n.insert(p);
                for (Point p{other.row - dy, other.col - dx}; inside(p); p.row -= dy, p.col -= dx)
                    n.insert(p);
            }
        }
        return int(n.size());
    }

    int maxCol, maxRow{0};
    unordered_map<Point, char> ants;
};

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    Antennas a;
    cout << "Day 8: Resonant Collinearity" << endl
         << "part 1   - " << a.nodes() << endl
         << "part 2   - " << a.harmonics() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
