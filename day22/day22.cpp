#include <pico/stdlib.h>

#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day22.txt"
};

static vector<uint32_t> secrets;

static uint32_t NewSecret(uint32_t secret) {
    secret = (secret ^ (secret << 6)) & 0xffffff;
    secret = (secret ^ (secret >> 5));
    return (secret ^ (secret << 11)) & 0xffffff;
}

static auto Part1() {
    uint64_t result = 0;
    for (auto secret : secrets) {
        for (int i = 0; i < 2000; i++) secret = NewSecret(secret);
        result += secret;
    }
    return result;
}

constexpr int MaxSeq = 19 * 19 * 19 * 19;
static array<uint32_t, MaxSeq> seqs{}, prices{};

static auto Part2() {
    for (auto secret : secrets) {
        int buyer = secret, price = secret % 10, seq = 0;
        for (int i = 0; i < 2000; i++) {
            secret = NewSecret(secret);
            int price2 = secret % 10, diff = price2 - price + 9;
            price = price2;
            seq = ((seq * 19) + diff) % MaxSeq;
            if (seqs[seq] != buyer) {
                seqs[seq] = buyer;
                prices[seq] += price;
            }
        }
    }
    return *max_element(prices.begin(), prices.end());
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    ifstream fi("day22.txt");
    string line;
    while (getline(fi, line)) secrets.push_back(stoul(line));
    cout << "Day 22: Monkey Market" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
