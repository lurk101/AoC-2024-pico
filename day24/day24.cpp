#include <pico/stdlib.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

struct instr {
    string s0, op, s1, d;
};

static vector<instr> instrs;
static map<string, uint64_t> wires;
static vector<string> zWires;
static vector<pair<string, int>> wireLevels;

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

static void Propagate() {
    bool done = false;
    while (!done) {
        for (const auto& [s0, op, s1, d] : instrs) {
            if (wires[s0] == -1 || wires[s1] == -1) continue;
            if (op[0] == 'X')
                wires[d] = wires[s0] ^ wires[s1];
            else if (op[0] == 'A')
                wires[d] = wires[s0] & wires[s1];
            else if (op[0] == 'O')
                wires[d] = wires[s0] | wires[s1];
        }
        done = true;
        for (const auto wire : zWires)
            if (wires[wire] == -1) {
                done = false;
                break;
            }
    }
}

static string FindInstr(string a, string oper, string b) {
    for (const auto& i : instrs)
        if (i.op == oper)
            if ((i.s0 == a && i.s1 == b) || (i.s0 == b && i.s1 == a)) return i.d;
    return "";
}

static void CheckSwap(string& a, string& b, vector<pair<string, string>>& swaps) {
    if (a.size() && a[0] == 'z') {
        swap(a, b);
        swaps.push_back({a, b});
    }
}

static uint64_t Part1() {
    sort(zWires.begin(), zWires.end());
    Propagate();
    uint64_t r = 0;
    for (int i = 0; i < zWires.size(); i++) r |= (wires[zWires[i]] << i);
    return r;
}

static string Part2() {
    vector<pair<string, string>> swaps;
    string in = "";
    for (int ix = 0; ix < 45; ix++) {
        string nn = (ix < 10) ? "0" + to_string(ix) : to_string(ix);
        string out = "", InxorSum = "", InandSum = "";
        string XxorY = FindInstr("x" + nn, "XOR", "y" + nn),
               XandY = FindInstr("x" + nn, "AND", "y" + nn);
        if (in.size()) {
            InandSum = FindInstr(in, "AND", XxorY);
            InxorSum = FindInstr(in, "XOR", XxorY);
            if (InandSum.empty()) {
                swap(XandY, XxorY);
                swaps.push_back({XandY, XxorY});
                InandSum = FindInstr(in, "AND", XxorY);
            }
            CheckSwap(XxorY, InxorSum, swaps);
            CheckSwap(XandY, InxorSum, swaps);
            CheckSwap(InandSum, InxorSum, swaps);
            out = FindInstr(InandSum, "OR", XandY);
        }
        if (out.size() && out[0] == 'z' && out != "z45") {
            swap(out, InxorSum);
            swaps.push_back({out, InxorSum});
        }
        in = in.empty() ? XandY : out;
    }
    vector<string> d;
    for (const auto [a, b] : swaps) {
        d.push_back(a);
        d.push_back(b);
    }
    sort(d.begin(), d.end());
    string s = "";
    for (const auto& d : d) {
        if (s.size()) s += ",";
        s += d;
    }
    return s;
}

static const vector<string> lines = {
#include "day24.txt"
};

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    ifstream fi("day24.txt");
    string line;
    int i = 0;
    for (;;) {
        string line(lines[i++]);
        if (line == "") break;
        vector<string> parts = Split(line, ": ");
        int v = stoi(parts[1]);
        wires[parts[0]] = v;
        if (parts[0][0] == 'z') zWires.push_back(parts[0]);
        wireLevels.push_back({parts[0], v});
    }
    while (i < lines.size()) {
        string line(lines[i++]);
        char s0[8], op[8], s1[8], d[8];
        sscanf(line.c_str(), "%s %s %s -> %s", s0, op, s1, d);
        instrs.push_back({string(s0), string(op), string(s1), string(d)});
        if (d[0] == 'z') zWires.push_back(string(d));
    }
    for (auto z : zWires) wires[z] = -1;
    for (auto i : instrs) {
        wires[i.s0] = -1;
        wires[i.s1] = -1;
        wires[i.d] = -1;
    }
    for (auto i : wireLevels) wires[i.first] = i.second;
    cout << "Day 24: Crossed Wires" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
