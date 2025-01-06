#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day13.txt"
};

struct point {
    int64_t x, y;
};

static vector<tuple<point, point, point>> claws;

static vector<string> Split(string s, string delimiter) {
    size_t posStart = 0, posEnd, delimLen = delimiter.length();
    string token;
    vector<string> r;
    while ((posEnd = s.find(delimiter, posStart)) != string::npos) {
        token = s.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        r.push_back(token);
    }
    r.push_back(s.substr(posStart));
    return r;
}

static int64_t Solve(bool gt100) {
    int64_t s = 0;
    for (auto [a, b, prize] : claws) {
        int64_t x1 = a.x, x2 = a.y, y1 = b.x, y2 = b.y, p1 = prize.x, p2 = prize.y, ky1 = -x2 * y1,
                ky2 = x1 * y2, kp1 = -x2 * p1, kp2 = x1 * p2, sumY = ky1 + ky2, sumP = kp1 + kp2;
        if (sumP % sumY) continue;
        int64_t rB = sumP / sumY;
        if ((p1 - rB * y1) % x1) continue;
        int64_t rA = (p2 - rB * y2) / x2;
        if (gt100 && (rA > 100 || rB > 100)) continue;
        if (rA < 0 || rB < 0) continue;
        s += rA * 3 + rB;
    }
    return s;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    point a, b, prize;
    for (const auto& line : lines) {
        if (line.size() == 0) {
            claws.push_back(make_tuple(a, b, prize));
            continue;
        }
        vector<string> tokens = Split(line, " ");
        if (tokens[0] == "Button") {
            if (tokens[1] == "A:") {
                sscanf(tokens[2].c_str(), "X+%lld", &a.x);
                sscanf(tokens[3].c_str(), "Y+%lld", &a.y);
            } else {
                sscanf(tokens[2].c_str(), "X+%lld", &b.x);
                sscanf(tokens[3].c_str(), "Y+%lld", &b.y);
            }
        } else {
            sscanf(tokens[1].c_str(), "X=%lld", &prize.x);
            sscanf(tokens[2].c_str(), "Y=%lld", &prize.y);
        }
    }
    claws.push_back(make_tuple(a, b, prize));
    auto p1 = Solve(true);
    for (auto& c : claws)
        c = make_tuple(get<0>(c), get<1>(c),
                       point({get<2>(c).x + 10000000000000, get<2>(c).y + 10000000000000}));
    auto p2 = Solve(false);
    cout << "Day 13: Claw Contraption" << endl
         << "Part 1   - " << p1 << endl
         << "Part 2   - " << p2 << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
