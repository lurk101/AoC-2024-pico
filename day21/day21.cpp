#include <pico/stdlib.h>

#include <chrono>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>
using namespace std;
using namespace chrono;

static const vector<string> codes = {
#include "day21.txt"
};
static map<tuple<int, int, int, int, int>, int64_t> memo;

static int64_t BestBot(const string& buttons, int nBots);

static int64_t BestPad(int r, int c, int destR, int destC, int nBots) {
    tuple<int, int, int, int, int> t({r, c, destR, destC, nBots});
    auto it = memo.find(t);
    if (it != memo.end()) return it->second;
    int64_t low = numeric_limits<int64_t>::max();
    struct Visit {
        int r, c;
        string buttons;
    };
    deque<Visit> q;
    q.push_back({r, c, ""});
    while (!q.empty()) {
        auto v = q.front();
        q.pop_front();
        if (v.r == destR && v.c == destC) {
            int64_t r = BestBot(v.buttons + "A", nBots - 1);
            if (r < low) low = r;
            continue;
        }
        if (v.r == 0 && v.c == 0)
            continue;
        else {
            if (v.r < destR)
                q.push_back({v.r + 1, v.c, v.buttons + "v"});
            else if (v.r > destR)
                q.push_back({v.r - 1, v.c, v.buttons + "^"});
            if (v.c < destC)
                q.push_back({v.r, v.c + 1, v.buttons + ">"});
            else if (v.c > destC)
                q.push_back({v.r, v.c - 1, v.buttons + "<"});
        }
    }
    memo[t] = low;
    return low;
}

static int64_t BestBot(const string& buttons, int nBots) {
    if (nBots == 1) return buttons.length();
    int64_t r = 0;
    const string keyPad = "X^A<v>";
    int row = 0;
    int col = 2;
    for (int i = 0; i < buttons.length(); i++)
        for (int nextR = 0; nextR < 2; nextR++)
            for (int nextC = 0; nextC < 3; nextC++)
                if (keyPad[nextR * 3 + nextC] == buttons[i]) {
                    r += BestPad(row, col, nextR, nextC, nBots);
                    row = nextR;
                    col = nextC;
                }
    return r;
}

static int64_t Best(int r, int c, int destR, int destC, int nBots) {
    int64_t low = numeric_limits<int64_t>::max();
    struct Visit {
        int r, c;
        string buttons;
    };
    deque<Visit> q;
    q.push_back({r, c, ""});
    while (!q.empty()) {
        auto v = q.front();
        q.pop_front();
        if (v.r == destR && v.c == destC) {
            int64_t r = BestBot(v.buttons + "A", nBots);
            if (r < low) low = r;
            continue;
        }
        if (v.r == 3 && v.c == 0)
            continue;
        else {
            if (v.r < destR)
                q.push_back({v.r + 1, v.c, v.buttons + "v"});
            else if (v.r > destR)
                q.push_back({v.r - 1, v.c, v.buttons + "^"});
            if (v.c < destC)
                q.push_back({v.r, v.c + 1, v.buttons + ">"});
            else if (v.c > destC)
                q.push_back({v.r, v.c - 1, v.buttons + "<"});
        }
    }
    return low;
}

static uint64_t Solve(int nBots) {
    int64_t s = 0;
    for (const auto& line : codes) {
        int64_t r = 0;
        const string keyPad = "789456123X0A";
        int row = 3, col = 2;
        for (auto c : line)
            for (int nextR = 0; nextR < 4; nextR++)
                for (int nextC = 0; nextC < 3; nextC++)
                    if (keyPad[nextR * 3 + nextC] == c) {
                        r += Best(row, col, nextR, nextC, nBots);
                        row = nextR;
                        col = nextC;
                    }
        s += r * stol(line);
    }
    return s;
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
