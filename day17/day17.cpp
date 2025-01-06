#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day17.txt"
};

static uint64_t rA, rB, rC;
static vector<uint8_t> insts;
static string output;

static vector<string> Split(string s, char delim) {
    size_t pos = 0, posEnd;
    string token;
    vector<string> r;
    while ((posEnd = s.find(delim, pos)) != string::npos) {
        token = s.substr(pos, posEnd - pos);
        pos = posEnd + 1;
        r.push_back(token);
    }
    r.push_back(s.substr(pos));
    return r;
}

static uint64_t ComboOp(uint8_t op) {
    switch (op) {
    case 0:
    case 1:
    case 2:
    case 3:
        return op;
    case 4:
        return rA;
    case 5:
        return rB;
    case 6:
        return rC;
    }
    return -1;
}

static vector<uint8_t> Run(uint64_t A) {
    vector<uint8_t> r;
    int ip = 0;
    rA = A;
    while (ip < insts.size()) {
        uint8_t opernd = insts[ip + 1], comboOp = ComboOp(opernd);
        switch (insts[ip]) {
        case 0:
            rA = rA / (1 << comboOp);
            break;
        case 1:
            rB = rB ^ opernd;
            break;
        case 2:
            rB = comboOp & 7;
            break;
        case 3:
            if (rA != 0) {
                ip = opernd;
                continue;
            }
            break;
        case 4:
            rB = rB ^ rC;
            break;
        case 5:
            r.push_back(comboOp & 7);
            break;
        case 6:
            rB = rA / (1 << comboOp);
            break;
        case 7:
            rC = rA / (1 << comboOp);
            break;
        }
        ip += 2;
    }
    return r;
}

static uint64_t Back(vector<uint8_t> pgm, uint64_t t, uint64_t cur) {
    for (int i = 0; i < 8; ++i) {
        uint64_t newCur = (cur << 3) + i;
        auto newPgm = Run(newCur);
        if (equal(newPgm.begin(), newPgm.end(),
                  vector<uint8_t>(pgm.begin() + t, pgm.end()).begin())) {
            if (t == 0) return newCur;
            uint64_t r = Back(pgm, t - 1, newCur);
            if (r != -1) return r;
        }
    }
    return -1;
}

static string Part1() {
    auto s = Run(rA);
    string r = "";
    for (const auto s1 : s) {
        if (r.size()) r += ",";
        r += to_string(s1);
    }
    return r;
}

static uint64_t Part2() { return Back(insts, insts.size() - 1, 0); }

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    ifstream fi("day17.txt");
    string line;
    getline(fi, line);
    vector<string> parts = Split(line, ' ');
    rA = stoi(parts[2]);
    getline(fi, line);
    parts = Split(line, ' ');
    rB = stoi(parts[2]);
    getline(fi, line);
    parts = Split(line, ' ');
    rC = stoi(parts[2]);
    getline(fi, line);
    getline(fi, line);
    parts = Split(line, ' ');
    output = parts[1];
    parts = Split(parts[1], ',');
    for (auto& part : parts) insts.push_back(stoi(part));
    cout << "Day 17: Chronospatial Computer" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
