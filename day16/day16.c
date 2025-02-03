/*  Advent of Code 2024 Day 16 Reindeer Maze
    Written 2025 by Eric Olson */

#include <limits.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t tic_start;

static const char* maze[] = {
#include "day16.txt"
};

static void tic() { tic_start = time_us_32(); }

static double toc() { return (time_us_32() - tic_start) / 1e6; }

static const int dirs[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};

static int isvertex(int i, int j) {
    if (maze[i][j] == 'S') return 1;
    if (maze[i][j] == 'E') return 1;
    if (maze[i][j] == '#') return 0;
    int r = 0;
    for (char d = 0; d < 4; d++) {
        int di = dirs[d][0], dj = dirs[d][1];
        if (maze[i + di][j + dj] != '#') r++;
    }
    if (r != 2) return 1;
    return 0;
}

static void vcount(int* pm, int* pstart, int* pstop) {
    int m = 0;
    int start, stop;
    for (int i = 0; i < 141; i++)
        for (int j = 0; j < 141; j++)
            if (isvertex(i, j)) {
                if (maze[i][j] == 'S')
                    start = m;
                else if (maze[i][j] == 'E')
                    stop = m;
                m++;
            }
    *pm = m;
    *pstart = start;
    *pstop = stop;
    return;
}

static void ntoij(int* pi, int* pj, int n) {
    int m = 0;
    for (int i = 0; i < 141; i++)
        for (int j = 0; j < 141; j++)
            if (isvertex(i, j)) {
                if (m == n) {
                    *pi = i;
                    *pj = j;
                    return;
                }
                m++;
            }
    printf("Tried to retrieve %d of %d vertices!\n", n, m);
    exit(1);
}

static int ijton(int hi, int hj) {
    int m = 0;
    for (int i = 0; i < 141; i++)
        for (int j = 0; j < 141; j++)
            if (isvertex(i, j)) {
                if (i == hi && j == hj) return m;
                m++;
            }
    printf("Tried to retrieve (%d,%d) but not a vertice!\n", hi, hj);
    exit(1);
}

typedef struct {
    int w[4], e[4];
    char de[4];
} vertspec;

static char dijtod(int di, int dj) {
    for (int d = 0; d < 4; d++)
        if (dirs[d][0] == di && dirs[d][1] == dj) return d;
    printf("Unknown direction (%d,%d)\n", di, dj);
    exit(1);
}

static int follow(int* pn, char* pd, int i, int j, int di, int dj) {
    int w = 0, e = 0;
    i += di;
    j += dj;
    w++;
    while (!isvertex(i, j)) {
        if (maze[i + di][j + dj] != '#') {
            i += di;
            j += dj;
            w++;
            continue;
        }
        int ti = dj;
        int tj = -di;
        if (maze[i + ti][j + tj] != '#') {
            di = ti;
            dj = tj;
            w += 1000;
            continue;
        }
        ti = -dj;
        tj = di;
        if (maze[i + ti][j + tj] != '#') {
            di = ti;
            dj = tj;
            w += 1000;
            continue;
        }
    }
    *pn = ijton(i, j);
    *pd = dijtod(di, dj);
    return w;
}

static void mkgraph(int n, vertspec vn[]) {
    for (int k = 0; k < n; k++) {
        int i, j;
        ntoij(&i, &j, k);
        for (int d = 0; d < 4; d++) {
            int di = dirs[d][0], dj = dirs[d][1];
            if (maze[i + di][j + dj] != '#') {
                int e;
                char de;
                int w = follow(&e, &de, i, j, di, dj);
                vn[k].w[d] = w;
                vn[k].e[d] = e;
                vn[k].de[d] = de;
            }
        }
    }
}

static void mkpath(int n, int pn[][4], vertspec vn[], int start) {
    for (int i = 0; i < n; i++)
        for (int d = 0; d < 4; d++) pn[i][d] = INT_MAX;
    pn[start][0] = 0;
    int done = 0;
    while (!done) {
        done = 1;
        for (int i = 0; i < n; i++) {
            for (int d = 0; d < 4; d++) {
                int t = pn[i][d];
                if (t < INT_MAX) {
                    if (vn[i].w[d] > 0) {
                        int tn = t + vn[i].w[d];
                        if (tn < pn[vn[i].e[d]][vn[i].de[d]]) {
                            pn[vn[i].e[d]][vn[i].de[d]] = tn;
                            done = 0;
                        }
                    }
                    int dl = (d + 1) % 4;
                    if (vn[i].w[dl] > 0) {
                        int tn = t + 1000;
                        if (tn < pn[i][dl]) {
                            pn[i][dl] = tn;
                            done = 0;
                        }
                    }
                    int dr = (d + 3) % 4;
                    if (vn[i].w[dr] > 0) {
                        int tn = t + 1000;
                        if (tn < pn[i][dr]) {
                            pn[i][dr] = tn;
                            done = 0;
                        }
                    }
                }
            }
        }
    }
    return;
}

static void parts(int* pp1, int* pp2, int n, vertspec vn[], int pn[][4], int stop) {
    int p1 = INT_MAX, dmin = 0;
    for (int d = 0; d < 4; d++) {
        int t = pn[stop][d];
        if (t < p1) {
            p1 = t;
            dmin = d;
        }
    }
    char sni[n][4], sno[n][4], cv[n];
    bzero(sni, n * 4 * sizeof(char));
    bzero(sno, n * 4 * sizeof(char));
    bzero(cv, n * sizeof(char));
    sni[stop][dmin] = 1;
    cv[stop] = 1;
    int p2 = 0;
    int done = 0;
    while (!done) {
        done = 1;
        for (int i = 0; i < n; i++) {
            for (int d = 0; d < 4; d++) {
                if (vn[i].w[d] > 0 && !sno[i][d])
                    if (sni[vn[i].e[d]][vn[i].de[d]])
                        if (pn[i][d] + vn[i].w[d] == pn[vn[i].e[d]][vn[i].de[d]]) {
                            p2 += vn[i].w[d] % 1000 - 1;
                            sno[i][d] = 1;
                            sni[i][d] = 1;
                            cv[i] = 1;
                            done = 0;
                        }
                int dl = (d + 3) % 4, dr = (d + 1) % 4;
                if (vn[i].w[dr] > 0 && !sni[i][dl]) {
                    if (sni[i][d]) {
                        if (pn[i][dl] + 1000 == pn[i][d]) {
                            sni[i][dl] = 1;
                            done = 0;
                        }
                    }
                }
                if (vn[i].w[dl] > 0 && !sni[i][dr]) {
                    if (sni[i][d]) {
                        if (pn[i][dr] + 1000 == pn[i][d]) {
                            sni[i][dr] = 1;
                            done = 0;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++)
        if (cv[i]) p2++;
    *pp1 = p1;
    *pp2 = p2;
    return;
}

int main() {
    stdio_init_all();
    printf("Advent of Code 2024 Day 16 Reindeer Maze\n\n");
    tic();
    int n, start, stop;
    vcount(&n, &start, &stop);
    vertspec vn[n];
    bzero(vn, sizeof(vertspec) * n);
    mkgraph(n, vn);
    int pn[n][4];
    mkpath(n, pn, vn, start);
    int p1, p2;
    parts(&p1, &p2, n, vn, pn, stop);
    printf("Part 1 The lowest score a Reindeer could get is %d\n", p1);
    printf("Part 2 The best viewing paths consist of %d tiles\n", p2);
    printf("\nTotal execution time %g seconds.\n", toc());
}
