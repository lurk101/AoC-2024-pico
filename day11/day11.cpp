#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day11.txt"
};

static unordered_map<string, uint64_t> pebbles;

static vector<string> Split(string s, string delimiter) {
    size_t pos = 0, posEnd, delimLen = delimiter.length();
    string token;
    vector<string> r;
    while ((posEnd = s.find(delimiter, pos)) != string::npos) {
        token = s.substr(pos, posEnd - pos);
        pos = posEnd + delimLen;
        r.push_back(token);
    }
    r.push_back(s.substr(pos));
    return r;
}

static inline void Insert(string key, uint64_t count) { pebbles[key] += count; }

static void NextGen() {
    unordered_map<string, uint64_t> prev;
    prev.swap(pebbles);
    for (auto& pebble : prev) {
        auto& [id, count] = pebble;
        if (id == "0") {
            Insert("1", count);
            continue;
        }
        int sz = int(id.size());
        if ((sz & 1) == 0) {
            auto s = id.substr(0, sz / 2);
            s = to_string(stoll(s));
            Insert(s, count);
            s = id.substr(sz / 2, sz / 2);
            s = to_string(stoll(s));
            Insert(s, count);
            continue;
        }
        Insert(to_string(2024 * stoll(id)), count);
    }
}

static uint64_t Part(int start, int blinks) {
    for (int i = start; i < blinks; i++) NextGen();
    uint64_t s = 0;
    for (auto pebble : pebbles) s += pebble.second;
    return s;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    ifstream fi("day11.txt");
    string line;
    while (getline(fi, line)) {
        vector<string> nums = Split(line, " ");
        for (auto n : nums) Insert(n, 1);
    }
    cout << "Day 11: Plutonian Pebbles" << endl
         << "Part 1   - " << Part(0, 25) << endl
         << "Part 2   - " << Part(25, 75) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
