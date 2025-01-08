#include <pico/stdlib.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day23.txt"
};

using cset = set<string>;

static cset c;                        // computers
static set<pair<string, string>> dx;  // directional connections
static map<string, cset> x;           // connections

static auto Part1() {
    int s = 0;
    set<tuple<string, string, string>> v;  // visited
    for (auto c1 : c)
        for (auto c2 : c) {
            if (c1 == c2) continue;
            if (!dx.contains({c1, c2})) continue;
            for (auto c3 : c) {
                if (c3 == c1 || c3 == c2) continue;
                if (c1[0] != 't' && c2[0] != 't' && c3[0] != 't') continue;
                if (dx.contains({c1, c2}) && dx.contains({c2, c3}) && dx.contains({c3, c1}) &&
                    !v.contains({c1, c2, c3})) {
                    v.insert({c1, c2, c3});
                    v.insert({c1, c3, c2});
                    v.insert({c2, c1, c3});
                    v.insert({c2, c3, c1});
                    v.insert({c3, c1, c2});
                    v.insert({c3, c2, c1});
                    ++s;
                }
            }
        }
    return s;
}

static cset Intersect(const cset& s1, const cset& s2) {
    cset s3;
    for (const string& s : s1)
        if (s2.contains(s)) s3.insert(s);
    return s3;
}

static cset LargestNet(cset& c, const map<string, cset>& x, cset& connected) {
    if (c.empty()) return connected;
    auto largest{connected};
    for (auto it = c.begin(); it != c.end(); it = c.erase(it)) {
        const auto& c1{*it};
        connected.insert(c1);
        auto s{Intersect(c, x.at(c1))};
        auto r{LargestNet(s, x, connected)};
        if (r.size() > largest.size()) largest = r;
        connected.erase(c1);
    }
    return largest;
}

static auto Part2() {
    cset connected, largest{LargestNet(c, x, connected)};
    vector<string> net{largest.begin(), largest.end()};
    sort(net.begin(), net.end());
    string s = "";
    for (const auto& c1 : net) {
        if (s.size()) s += ",";
        s += c1;
    }
    return s;
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    for (const auto& line : lines) {
        auto p0 = line.substr(0, 2), p1 = line.substr(3);
        c.insert(p0);
        c.insert(p1);
        x[p0].insert(p1);
        x[p1].insert(p0);
        dx.insert({p0, p1});
        dx.insert({p1, p0});
    }
    cout << "Day 23: LAN Party" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
