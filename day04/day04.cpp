#include <pico/stdlib.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> m = {
#include "day04.txt"
};

static int H(int i, int j, const char* w) {
    return (j <= m.size() - 4) ? (memcmp(&m[i][j], w, 4) == 0 ? 1 : 0) : 0;
}

static int V(int i, int j, const char* w) {
    if (i <= m.size() - 4) {
        int count = 1;
        for (int k = 0; k < 4; k++)
            if (m[i + k][j] != w[k]) count = 0;
        return count;
    }
    return 0;
}

static int diagL(int i, int j, const char* w) {
    if (i <= m.size() - 4 && j <= m.size() - 4) {
        int count(1);
        for (int k = 0; k < 4; k++)
            if (m[i + k][j + k] != w[k]) count = 0;
        return count;
    }
    return 0;
}

static int diagR(int i, int j, const char* w) {
    if (i <= m.size() - 4 && j >= 4 - 1) {
        int count(1);
        for (int k = 0; k < 4; k++)
            if (m[i + k][j - k] != w[k]) count = 0;
        return count;
    }
    return 0;
}

static int part1() {
    int count = 0;
    for (int i = 0; i < m.size(); i++)
        for (int j = 0; j < m.size(); j++)
            count += H(i, j, "XMAS") + H(i, j, "SAMX") + V(i, j, "XMAS") + V(i, j, "SAMX") +
                     diagL(i, j, "XMAS") + diagL(i, j, "SAMX") + diagR(i, j, "XMAS") +
                     diagR(i, j, "SAMX");
    return count;
}

static int part2() {
    int count(0);
    for (int i = 0; i <= m.size() - 3; i++) {
        for (int j = 0; j <= m.size() - 3; j++) {
            int ok(0);
            if (m[i + 1][j + 1] == 'A') {
                ok = 1;
                if (!((m[i][j] == 'M' && m[i + 2][j + 2] == 'S') ||
                      (m[i][j] == 'S' && m[i + 2][j + 2] == 'M')))
                    ok = 0;
                if (!((m[i + 2][j] == 'M' && m[i][j + 2] == 'S') ||
                      (m[i + 2][j] == 'S' && m[i][j + 2] == 'M')))
                    ok = 0;
            }
            count += ok;
        }
    }
    return count;
}

int main() {
    stdio_init_all();
    auto start = high_resolution_clock::now();
    cout << "Day 4: Ceres Search" << endl
         << "part 1   - " << part1() << endl
         << "part 2   - " << part2() << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
