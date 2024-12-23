#include <pico/stdlib.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static set<string> c;                 // computers
static set<pair<string, string>> dx;  // directional connections
static map<string, set<string>> x;    // connections

static int Part1() {
    int sol = 0;
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
                    ++sol;
                }
            }
        }
    return sol;
};

static set<string> And(const set<string>& s1, const set<string>& s2) {
    set<string> intersect;
    for (const string& s : s1)
        if (s2.contains(s)) intersect.insert(s);
    return intersect;
}

static set<string> LargestNet(set<string>& c, const map<string, set<string>>& x,
                              set<string>& connected) {
    if (c.empty()) return connected;
    set<string> largest{connected};
    for (auto it = c.begin(); it != c.end();) {
        connected.insert(*it);
        auto s{And(c, x.at(*it))};
        auto r{LargestNet(s, x, connected)};
        if (r.size() > largest.size()) largest = r;
        connected.erase(*it);
        it = c.erase(it);
    }
    return largest;
}

static string Part2() {
    set<string> connected, largest{LargestNet(c, x, connected)};
    vector<string> net{largest.begin(), largest.end()};
    sort(net.begin(), net.end());
    string s = "";
    for (int i = 0; i < net.size(); i++) {
        s += net[i];
        if (i < net.size() - 1) s += ",";
    }
    return s;
}

static const vector<string> lines = {
#include "day23.txt"
};

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
