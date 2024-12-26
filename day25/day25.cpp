#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<uint32_t> keys, locks;

static const vector<string> lines = {
#include "day25.txt"
};

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    string line;
    int y = 0;
    uint32_t keyOrLock = 0;
    for (const auto& line : lines) {
        if (line.empty()) {
            y = 0;
            keyOrLock = 0;
            continue;
        }
        if ((y > 0) && (y < 6)) {
            for (int x = 0; x < line.size(); x++) {
                keyOrLock <<= 1;
                if (line[x] == '#') keyOrLock |= 1;
            }
        } else {
            if (y == 6) {
                if (line[0] == '#')
                    locks.push_back(keyOrLock);
                else
                    keys.push_back(keyOrLock);
            }
        }
        y++;
    }
    int sum = 0;
    for (const auto& lock : locks)
        for (const auto& key : keys)
            if ((lock & key) == 0) sum++;
    cout << "Day 25: Code Chronicle" << endl
         << "Part 1   - " << sum << endl
         << "Part 2   - free star" << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
