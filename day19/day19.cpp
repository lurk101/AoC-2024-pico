#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> patterns, designs;

static vector<string> Split(const string& s, const string delim) {
    size_t pos = 0, posEnd;
    string token;
    vector<string> r;
    while ((posEnd = s.find(delim, pos)) != string::npos) {
        token = s.substr(pos, posEnd - pos);
        pos = posEnd + delim.size();
        r.push_back(token);
    }
    r.push_back(s.substr(pos));
    return r;
}

static int Part1() {
    int s = 0;
    for (string d : designs) {
        int n = d.size();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        for (int i = 1; i <= n; i++)
            for (string p : patterns) {
                int len = p.size();
                if (i >= len && d.substr(i - len, len) == p) dp[i] = dp[i] || dp[i - len];
            }
        if (dp[n]) s++;
    }
    return s;
}

static uint64_t Part2() {
    uint64_t s = 0;
    for (string d : designs) {
        int n = d.size();
        vector<uint64_t> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= n; i++)
            for (string p : patterns) {
                int len = p.size();
                if (i >= len && d.substr(i - len, len) == p) dp[i] += dp[i - len];
            }
        s += dp[n];
    }
    return s;
}

static const vector<string> lines = {
#include "day19.txt"
};

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    int i = 0;
    string line = lines[i++];
    patterns = Split(line, ", ");
    line = lines[i++];
    while (i < lines.size()) designs.push_back(lines[i++]);
    cout << "Day 19: Linen Layout" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
