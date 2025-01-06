#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const string diskmap =
#include "day09.txt"
    ;

static uint64_t part1() {
    vector<int> blocks;
    int nFiles{0}, isFree{false};
    for (const auto c : diskmap) {
        const int n = c - '0';
        if (isFree) {
            for (int i = 0; i < n; i++) blocks.push_back(-1);
            isFree = false;
        } else {
            for (int i = 0; i < n; i++) blocks.push_back(nFiles);
            isFree = true;
            nFiles++;
        }
    }
    for (int lIdx = 0, rIdx = int(blocks.size() - 1); lIdx < rIdx; lIdx++, rIdx--) {
        while (blocks[lIdx] != -1) lIdx++;
        while (blocks[rIdx] == -1) rIdx--;
        swap(blocks[lIdx], blocks[rIdx]);
    }
    uint64_t chkSum = 0;
    for (const auto [idx, val] : views::enumerate(blocks)) {
        if (val == -1) break;
        chkSum += idx * (val);
    }
    return chkSum;
}

static uint64_t part2() {
    struct Block {
        Block(const int startIdx, const int size, const int value)
            : startIdx(startIdx), size(size), value(value) {}
        int startIdx, size, value;
        tuple<Block, Block> frag(const int sizeIn, const int valIn) const {
            return {Block(startIdx, sizeIn, valIn), Block(startIdx + sizeIn, size - sizeIn, -1)};
        }
    };
    vector<Block> blocks;
    int nFiles{0}, p{0}, isFree{false};
    for (const auto c : diskmap) {
        const int n = c - '0';
        if (isFree) {
            blocks.emplace_back(p, n, -1);
            isFree = false;
        } else {
            blocks.emplace_back(p, n, nFiles);
            isFree = true;
            nFiles++;
        }
        p += n;
    }
    int idx{int(blocks.size() - 1)};
    while (idx > 0) {
        if (blocks[idx].value == -1) {
            idx--;
            continue;
        }
        for (int idx2 = 0; idx2 < idx; idx2++)
            if (blocks[idx2].value == -1 && blocks[idx2].size >= blocks[idx].size) {
                const auto [b1, b2] = blocks[idx2].frag(blocks[idx].size, blocks[idx].value);
                blocks[idx2] = b1;
                blocks[idx].value = -1;
                blocks.insert(begin(blocks) + idx2 + 1, b2);
                break;
            }
        idx--;
    }
    uint64_t chkSum{0};
    idx = 0;
    for (const auto& b : blocks)
        if (b.value != -1)
            for (int i = 0; i < b.size; i++, idx++) chkSum += idx * b.value;
        else
            idx += b.size;
    return chkSum;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    cout << "Day 9: Disk Fragmenter" << endl
         << "Part 1   - " << part1() << endl
         << "Part 2   - " << part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
