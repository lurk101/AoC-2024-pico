#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> lines = {
#include "day03.txt"
};

static vector<string> memories;

static int Part1() {
    int count = 0;
    smatch match;
    regex expr(R"(mul\((\d+)+,(\d+)+\))");
    for (auto mem : memories)
        while (regex_search(mem, match, expr)) {
            count += stoi(match[1].str()) * stoi(match[2].str());
            mem = match.suffix().str();
        }
    return count;
}

static int Part2() {
    bool mulOn = true;
    int count = 0;
    smatch match;
    regex expr(R"(mul\((\d+)+,(\d+)+\))");
    regex on(R"(do\(\))");
    regex off(R"(don't\(\))");
    regex combo(R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))");
    for (auto mem : memories)
        while (regex_search(mem, match, combo)) {
            if (!mulOn && regex_match(match.str(), on)) {
                mulOn = true;
                continue;
            }
            if (mulOn && regex_match(match.str(), off)) {
                mulOn = false;
                continue;
            }
            if (mulOn && regex_match(match.str(), expr))
                count += stoi(match[1].str()) * stoi(match[2].str());
            mem = match.suffix().str();
        }
    return count;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    ifstream fi("day03.txt");
    string line;
    while (getline(fi, line)) memories.push_back(line);
    cout << "Day 3: Mull It Over" << endl
         << "Part 1   - " << Part1() << endl
         << "Part 2   - " << Part2() << endl
         << "Run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
