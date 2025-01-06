#include <pico/stdlib.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day05.txt"
};

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    string line;
    int ix = 0;
    unordered_map<int8_t, unordered_set<int8_t>> p;
    while (lines[ix] != "") {
        const auto& line(lines[ix++]);
        p[stoi(line.substr(0, 2))].insert(stoi(line.substr(3, 2)));
    }
    ix++;
    int part1(0), part2(0);
    while (ix < lines.size()) {
        const auto& line(lines[ix++]);
        vector<int8_t> ordered;
        for (int i = 0; i < line.length(); i += 3) ordered.push_back(stoi(line.substr(i, 2)));
        auto unOrdered(ordered);
        sort(ordered.begin(), ordered.end(),
             [&p](int8_t l, int8_t r) { return p[l].find(r) != p[l].end(); });
        if (unOrdered == ordered)
            part1 += ordered[ordered.size() / 2];
        else
            part2 += ordered[ordered.size() / 2];
    }
    cout << "Day 5: Print Queue" << endl
         << "Part 1   - " << part1 << endl
         << "Part 2   - " << part2 << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
