#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

static stringstream nums({
#include "day11.txt"
});

static unordered_map<string, uint64_t> pebbles;

static void NextGen() {
    unordered_map<string, uint64_t> prev;
    prev.swap(pebbles);
    for (auto& pebble : prev) {
        auto& [id, count] = pebble;
        if (id == "0") {
            pebbles["1"] += count;
            continue;
        }
        int sz = int(id.size());
        if ((sz & 1) == 0) {
            auto s = id.substr(0, sz / 2);
            s = to_string(stoll(s));
            pebbles[s] += count;
            s = id.substr(sz / 2, sz / 2);
            s = to_string(stoll(s));
            pebbles[s] += count;
            continue;
        }
        pebbles[to_string(2024 * stoll(id))] += count;
    }
}

static auto Part(int start, int next) {
    for (int i = start; i < next; i++) NextGen();
    uint64_t s = 0;
    for (auto pebble : pebbles) s += pebble.second;
    return s;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    string num;
    while (nums >> num) pebbles[num] = 1;
    cout << "Day 11: Plutonian Pebbles" << endl
         << "Part 1   - " << Part(0, 25) << endl
         << "Part 2   - " << Part(25, 75) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
