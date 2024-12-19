#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;
using namespace chrono;

static auto getNumbers(string_view const line, char delim) {
    vector<size_t> parts;
    size_t strt{};
    auto const len = line.size();
    while (strt < len) {
        size_t end = line.find(delim, strt);
        if (end == string_view::npos) end = len;
        size_t num{};
        from_chars(line.data() + strt, line.data() + end, num);
        parts.push_back(num);
        strt = end + 1;
    }
    return parts;
}

static constexpr bool divMod(size_t dividend, size_t divisor, size_t& quotient, size_t& remainder) {
    if (divisor == 0) return false;
    quotient = dividend / divisor;
    remainder = dividend % divisor;
    return true;
}

static bool valid(size_t i, size_t testVal, vector<size_t> const& eq, bool concat) {
    auto const curr = eq[i];
    if (i == 0) return curr == testVal;
    size_t divVal, modVal;
    if (divMod(testVal, curr, divVal, modVal) && modVal == 0 && valid(i - 1, divVal, eq, concat))
        return true;
    if (testVal > curr && valid(i - 1, testVal - curr, eq, concat)) return true;
    if (concat) {
        size_t multiplier{1}, temp{curr};
        while (temp > 0) {
            multiplier *= 10;
            temp /= 10;
        }
        if (divMod(testVal, multiplier, divVal, modVal) && modVal == curr &&
            valid(i - 1, divVal, eq, concat)) {
            return true;
        }
    }
    return false;
}

static pair<size_t, size_t> solve(vector<string_view> const& in) {
    size_t part1(0);
    size_t part2(0);
    for (auto const line : in) {
        auto const colon{line.find(':')};
        auto const eq_view{line.substr(colon + 2)};
        auto const eq{getNumbers(eq_view, ' ')};
        auto const eq_len{eq.size()};
        size_t val = 0;
        from_chars(line.data(), line.data() + colon, val);
        if (valid(eq_len - 1, val, eq, false)) part1 += val;
        if (valid(eq_len - 1, val, eq, true)) part2 += val;
    }
    return {part1, part2};
}

static const string buffer =
#include "day07.txt"
    ;

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    vector<string_view> in;
    size_t strt = 0;
    for (size_t i = 0; i < buffer.size(); ++i) {
        if (buffer[i] == '\n' || buffer[i] == '\r') {
            in.emplace_back(buffer.data() + strt, i - strt);
            strt = i + 1;
        }
    }
    if (strt < buffer.size()) in.emplace_back(buffer.data() + strt, buffer.size() - strt);
    auto const [p1, p2]{solve(in)};
    cout << "Day 7: Bridge Repair" << endl
         << "part 1   - " << p1 << endl
         << "part 2   - " << p2 << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
