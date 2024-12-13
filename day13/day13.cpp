#include <chrono>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "pico/stdlib.h"

using namespace std;
using namespace chrono;

static vector<tuple<pair<int64_t, int64_t>, pair<int64_t, int64_t>, pair<int64_t, int64_t>>> claw;

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
    for (auto [a, b, prize] : claw) {
        int64_t rA = 0, rB = 0, x1 = a.first, x2 = a.second, y1 = b.first, y2 = b.second,
                p1 = prize.first, p2 = prize.second, ky1 = -x2 * y1, ky2 = x1 * y2, kp1 = -x2 * p1,
                kp2 = x1 * p2, sumY = ky1 + ky2, sumP = kp1 + kp2;
        if (sumP % sumY == 0)
            rB = sumP / sumY;
        else
            continue;
        if ((p1 - rB * y1) % x1 == 0)
            rA = (p2 - rB * y2) / x2;
        else
            continue;
        if (gt100 && (rA > 100 || rB > 100)) continue;
        if (rA < 0 || rB < 0) continue;
        s += rA * 3 + rB;
    }
    return s;
}

static const vector<string> lines = {
#include "day13.txt"
};

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    string line;
    pair<int64_t, int64_t> a, b, prize;
    for (auto& line : lines) {
        if (line.size() == 0) {
            claw.push_back(make_tuple(a, b, prize));
            a = make_pair(0, 0);
            b = make_pair(0, 0);
            prize = make_pair(0, 0);
            continue;
        }
        vector<string> tokens = Split(line, " ");
        if (tokens[0] == "Button" && tokens[1] == "A:") {
            sscanf(tokens[2].c_str(), "X+%lld", &a.first);
            sscanf(tokens[3].c_str(), "Y+%lld", &a.second);
        } else if (tokens[0] == "Button" && tokens[1] == "B:") {
            sscanf(tokens[2].c_str(), "X+%lld", &b.first);
            sscanf(tokens[3].c_str(), "Y+%lld", &b.second);
        } else if (tokens[0] == "Prize:") {
            sscanf(tokens[1].c_str(), "X=%lld", &prize.first);
            sscanf(tokens[2].c_str(), "Y=%lld", &prize.second);
        }
    }
    claw.push_back(make_tuple(a, b, prize));
    auto p1(Solve(true));
    for (auto& c : claw)
        c = make_tuple(
            get<0>(c), get<1>(c),
            make_pair(get<2>(c).first + 10000000000000, get<2>(c).second + 10000000000000));
    auto p2(Solve(false));
    cout << "Day 13: Claw Contraption" << endl
         << "Part 1   - " << p1 << endl
         << "Part 2   - " << p2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
