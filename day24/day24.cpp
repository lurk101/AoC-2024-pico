#include <pico/stdlib.h>

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day24.txt"
};

struct gate {
    string in0, op, in1, out;  // input 0, operator, input 1, output
};

static vector<gate> gates;
static map<string, optional<bool>> wires;
static vector<string> zWires;

static void Propagate() {
    bool run = true;
    while (run) {  // repeat till all z wires are resolved
        for (const auto& [in0, op, in1, d] : gates) {
            if (!(wires[in0] && wires[in1])) continue;
            if (op[0] == 'X')  // XOR
                wires[d] = wires[in0].value() != wires[in1].value();
            else if (op[0] == 'A')  // AND
                wires[d] = wires[in0].value() && wires[in1].value();
            else  // if (op[0] == 'O')  // OR
                wires[d] = wires[in0].value() || wires[in1].value();
        }
        run = false;
        for (const auto wire : zWires)
            if (!wires[wire]) {
                run = true;
                break;
            }
    }
}

static auto Part1() {
    sort(zWires.begin(), zWires.end());
    Propagate();
    uint64_t r = 0;
    for (int i = 0; i < zWires.size(); r |= (wires[zWires[i]].value() ? 1LL : 0LL) << i++)
        ;
    return r;
}

static string GateOutWire(const string a, const string oper, const string b) {
    for (const auto& gate : gates)
        if (gate.op == oper)
            if ((gate.in0 == a && gate.in1 == b) || (gate.in0 == b && gate.in1 == a))
                return gate.out;
    return "";
}

static vector<pair<string, string>> swaps;

static void TrySwap(string& a, string& b) {
    if (a.size() && a[0] == 'z') {
        swap(a, b);
        swaps.push_back({a, b});
    }
}

static auto Part2() {
    string In = "";
    for (int ix = 0; ix < 45; ix++) {
        string Out = "", InxorSum = "", InandSum = "";
        const string nn(format("{:02}", ix));
        string XxorY = GateOutWire("x" + nn, "XOR", "y" + nn);
        string XandY = GateOutWire("x" + nn, "AND", "y" + nn);
        if (In.size()) {
            InandSum = GateOutWire(In, "AND", XxorY);
            InxorSum = GateOutWire(In, "XOR", XxorY);
            if (InandSum.empty()) {
                swap(XandY, XxorY);
                swaps.push_back({XandY, XxorY});
                InandSum = GateOutWire(In, "AND", XxorY);
            }
            TrySwap(XxorY, InxorSum);
            TrySwap(XandY, InxorSum);
            TrySwap(InandSum, InxorSum);
            Out = GateOutWire(InandSum, "OR", XandY);
        }
        if (Out.size() && (Out[0] == 'z') && (Out != "z45")) {
            swap(Out, InxorSum);
            swaps.push_back({Out, InxorSum});
        }
        if (swaps.size() >= 4) break;
        In = In.empty() ? XandY : Out;
    }
    vector<string> d;
    for (const auto& [a, b] : swaps) {
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

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    ifstream fi("day24.txt");
    vector<pair<string, bool>> wireLevels;
    string line;
    while (getline(fi, line)) {
        if (line == "") break;
        wireLevels.push_back({line.substr(0, 3), stoi(line.substr(4)) == 1});
    }
    string in0;
    while (fi >> in0) {
        string op, in1, d;
        fi >> op >> in1 >> d >> d;
        gates.push_back({in0, op, in1, d});
        if (d[0] == 'z') zWires.push_back(d);
    }
    for (const auto& l : wireLevels) wires[l.first] = l.second;
    cout << "Day 24: Crossed Wires" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
