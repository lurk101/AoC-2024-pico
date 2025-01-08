#include <pico/stdlib.h>

#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

static const vector<string> codes = {
#include "day21.txt"
};

typedef tuple<int8_t, int8_t, int8_t, int8_t, int8_t> key;

static map<key, int64_t> memo;

static int64_t BestDirKey(const string& buttons, int nBots);

static int64_t BestNumKey(int r, int c, int dstR, int dstC, int nBots) {
    key k({r, c, dstR, dstC, nBots});
    auto it = memo.find(k);
    if (it != memo.end()) return it->second;
    int64_t low = numeric_limits<int64_t>::max();
    struct Visit {
        int r, c;
        string buttons;
    };
    queue<Visit> q;
    q.push({r, c, ""});
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        if (v.r == dstR && v.c == dstC) {
            low = min(low, BestDirKey(v.buttons + "A", nBots - 1));
            continue;
        }
        if (v.r == 0 && v.c == 0)
            continue;
        if (v.r < dstR)
            q.push({v.r + 1, v.c, v.buttons + "v"});
        else if (v.r > dstR)
            q.push({v.r - 1, v.c, v.buttons + "^"});
        if (v.c < dstC)
            q.push({v.r, v.c + 1, v.buttons + ">"});
        else if (v.c > dstC)
            q.push({v.r, v.c - 1, v.buttons + "<"});
    }
    memo[k] = low;
    return low;
}

static int64_t BestDirKey(const string& buttons, int nBots) {
    if (nBots == 1) return buttons.length();
    int64_t rslt = 0;
    static const vector<string> dirKeyPad = {"X^A", "<v>"};
    int r = 0, c = 2;
    for (auto ch : buttons)
        for (int nR = 0; nR < 2; nR++)
            for (int nC = 0; nC < 3; nC++)
                if (dirKeyPad[nR][nC] == ch) {
                    rslt += BestNumKey(r, c, nR, nC, nBots);
                    r = nR;
                    c = nC;
                }
    return rslt;
}

static int64_t Best(int r, int c, int destR, int destC, int nBots) {
    int64_t low = numeric_limits<int64_t>::max();
    struct Visit {
        int r, c;
        string buttons;
    };
    queue<Visit> q;
    q.push({r, c, ""});
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        if (v.r == destR && v.c == destC) {
            low = min(low, BestDirKey(v.buttons + "A", nBots));
            continue;
        }
        if (v.r == 3 && v.c == 0)
            continue;
        if (v.r < destR)
            q.push({v.r + 1, v.c, v.buttons + "v"});
        else if (v.r > destR)
            q.push({v.r - 1, v.c, v.buttons + "^"});
        if (v.c < destC)
            q.push({v.r, v.c + 1, v.buttons + ">"});
        else if (v.c > destC)
            q.push({v.r, v.c - 1, v.buttons + "<"});
    }
    return low;
}

static uint64_t Solve(int nBots) {
    int64_t rslt = 0;
    for (const auto& code : codes) {
        int64_t b = 0;
        const vector<string> numKeyPad = {"789", "456", "123", "X0A"};
        int r = 3, c = 2;
        for (auto ch : code)
            for (int nR = 0; nR < 4; nR++)
                for (int nC = 0; nC < 3; nC++)
                    if (numKeyPad[nR][nC] == ch) {
                        b += Best(r, c, nR, nC, nBots);
                        r = nR;
                        c = nC;
                    }
        rslt += b * stol(code);
    }
    return rslt;
}

int main() {
    stdio_init_all();
    auto strt = high_resolution_clock::now();
    cout << "Day 21: Keypad Conundrum" << endl
         << "Part 1   - " << Solve(3) << endl
         << "Part 2   - " << Solve(26) << endl
         << "run time - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0
         << " ms." << endl;
}
