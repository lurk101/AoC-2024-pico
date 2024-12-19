#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> patterns;
static vector<string> designs;

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

static uint64_t Ways(const string& design, int partNum) {
    int n = design.size();
    vector<uint64_t> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; i++)
        for (string p : patterns) {
            int len = p.size();
            if (partNum == 1) {
                if (i >= len && design.substr(i - len, len) == p) dp[i] = dp[i] || dp[i - len];
            } else if (i >= len && design.substr(i - len, len) == p)
                dp[i] += dp[i - len];
        }
    return dp[n];
}

static uint64_t Part(int partNum) {
    uint64_t s = 0;
    for (string d : designs) s += Ways(d, partNum);
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
    while (i < lines.size()) {
        line = lines[i++];
        designs.push_back(line);
    }
    cout << "Day 19: Linen Layout" << endl
         << "Part 1   - " << Part(1) << endl
         << "Part 2   - " << Part(2) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
