#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

unordered_map<string, uint64_t> pebbles;

static vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> r;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        r.push_back(token);
    }
    r.push_back(s.substr(pos_start));
    return r;
}

static inline void Insert(string& key, uint64_t count) { pebbles[key] += count; }

static void NextGen() {
    unordered_map<string, uint64_t> prev;
    prev.swap(pebbles);
    for (auto& x : prev) {
        auto& [name, count] = x;
        if (name == "0") {
            string s1("1");
            Insert(s1, count);
            continue;
        }
        int sz = int(name.size());
        if ((sz & 1) == 0) {
            auto s1 = name.substr(0, sz / 2);
            s1 = to_string(stoll(s1, NULL, 10));
            auto s2 = name.substr(sz / 2, sz / 2);
            s2 = to_string(stoll(s2, NULL, 10));
            Insert(s1, count);
            Insert(s2, count);
            continue;
        }
        auto s1 = to_string(2024 * stoll(name));
        Insert(s1, count);
    }
}

static uint64_t Part(int start, int blinks) {
    for (int i = start; i < blinks; i++) NextGen();
    uint64_t s = 0ULL;
    for (auto x : pebbles) s += x.second;
    return s;
}

static const string line =
#include "day11.txt"
    ;

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    vector<string> nums = split(line, " ");
    for (auto x : nums) Insert(x, 1);
    cout << "Day 11: Plutonian Pebbles" << endl
         << "Part 1   - " << Part(0, 25) << endl
         << "Part 2   - " << Part(25, 75) << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
