#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day07.txt"
};

static vector<uint64_t> GetNums(const string& s, char delim) {
    uint64_t pos = 0, posEnd;
    string token;
    vector<uint64_t> r;
    while ((posEnd = s.find(delim, pos)) != string::npos) {
        token = s.substr(pos, posEnd - pos);
        pos = posEnd + 1;
        r.push_back(stoll(token));
    }
    token = s.substr(pos);
    r.push_back(stoll(token));
    return r;
}

static constexpr bool DivMod(uint64_t dividend, uint64_t divisor, uint64_t& quotient,
                             uint64_t& remainder) {
    if (divisor == 0) return false;
    quotient = dividend / divisor;
    remainder = dividend % divisor;
    return true;
}

static bool Valid(uint64_t i, uint64_t testVal, vector<uint64_t> const& eq, bool concat) {
    auto const curr = eq[i];
    if (i == 0) return curr == testVal;
    uint64_t divVal, modVal;
    if (DivMod(testVal, curr, divVal, modVal) && modVal == 0 && Valid(i - 1, divVal, eq, concat))
        return true;
    if (testVal > curr && Valid(i - 1, testVal - curr, eq, concat)) return true;
    if (concat) {
        uint64_t multiplier{1}, temp{curr};
        while (temp > 0) {
            multiplier *= 10;
            temp /= 10;
        }
        if (DivMod(testVal, multiplier, divVal, modVal) && modVal == curr &&
            Valid(i - 1, divVal, eq, concat)) {
            return true;
        }
    }
    return false;
}

static pair<uint64_t, uint64_t> solve(vector<string> const& in) {
    uint64_t part1(0);
    uint64_t part2(0);
    for (auto const line : in) {
        auto const colon{line.find(':')};
        auto const eq_view{line.substr(colon + 2)};
        auto const eq{GetNums(eq_view, ' ')};
        auto const eq_len{eq.size()};
        uint64_t val = stoll(string(line));
        if (Valid(eq_len - 1, val, eq, false)) part1 += val;
        if (Valid(eq_len - 1, val, eq, true)) part2 += val;
    }
    return {part1, part2};
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    ifstream fi("day07.txt");
    string line;
    vector<string> equations;
    while (getline(fi, line)) equations.push_back(line);
    auto const [p1, p2]{solve(equations)};
    cout << "Day 7: Bridge Repair" << endl
         << "part 1   - " << p1 << endl
         << "part 2   - " << p2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
