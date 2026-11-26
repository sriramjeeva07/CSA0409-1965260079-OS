#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 8
#define MAXCYL 199

static int req[N] = {98, 183, 37, 122, 14, 124, 65, 67};

static int cmp(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int movement(int start, const int order[], int n) {
    int pos = start, total = 0;
    for (int i = 0; i < n; ++i) {
        total += abs(order[i] - pos);
        pos = order[i];
    }
    return total;
}

static void show(const char *name, int start, const int order[], int n, int extra) {
    printf("%-7s %d", name, start);
    for (int i = 0; i < n; ++i) printf(" %d", order[i]);
    printf(" -> %d cylinders\n", movement(start, order, n) + extra);
}

static void sstf(int head, int order[]) {
    int done[N] = {0};
    int pos = head;
    for (int k = 0; k < N; ++k) {
        int best = -1, nearest = MAXCYL + 1;
        for (int i = 0; i < N; ++i) {
            if (!done[i]) {
                int d = abs(req[i] - pos);
                if (d < nearest) {
                    nearest = d;
                    best = i;
                }
            }
        }
        order[k] = req[best];
        done[best] = 1;
        pos = req[best];
    }
}

int main(void) {
    int sorted[N], order[N + 2], n = 0;
    memcpy(sorted, req, sizeof sorted);
    qsort(sorted, N, sizeof sorted[0], cmp);

    show("FCFS", 53, req, N, 0);
    sstf(53, order);
    show("SSTF", 53, order, N, 0);

    /* SCAN: move down to cylinder zero, then reverse. */
    n = 0;
    for (int i = N - 1; i >= 0; --i) {
        if (sorted[i] < 53) order[n++] = sorted[i];
    }
    order[n++] = 0;
    for (int i = 0; i < N; ++i) {
        if (sorted[i] > 53) order[n++] = sorted[i];
    }
    show("SCAN", 53, order, n, 0);

    /* LOOK: reverse at the last request, not at cylinder zero. */
    n = 0;
    for (int i = N - 1; i >= 0; --i) {
        if (sorted[i] < 53) order[n++] = sorted[i];
    }
    for (int i = 0; i < N; ++i) {
        if (sorted[i] > 53) order[n++] = sorted[i];
    }
    show("LOOK", 53, order, n, 0);

    /* C-SCAN: upward sweep to 199, jump to zero, then serve lower requests. */
    n = 0;
    for (int i = 0; i < N; ++i) {
        if (sorted[i] > 53) order[n++] = sorted[i];
    }
    order[n++] = MAXCYL;
    int total = movement(53, order, n);
    int lower[N], m = 0;
    for (int i = 0; i < N; ++i) {
        if (sorted[i] < 53) lower[m++] = sorted[i];
    }
    if (m > 0) {
        total += MAXCYL; /* head returns from 199 to zero */
        for (int i = 0; i < m; ++i) total += (i == 0 ? lower[i] : lower[i] - lower[i - 1]);
    }
    printf("C-SCAN  53");
    for (int i = 0; i < n; ++i) printf(" %d", order[i]);
    printf(" -> wrap to 0");
    for (int i = 0; i < m; ++i) printf(" %d", lower[i]);
    printf(" -> %d cylinders (wrap %d included)\n", total, MAXCYL);

    /* C-LOOK: downward sweep, then jump from lowest to highest pending request. */
    n = 0;
    for (int i = N - 1; i >= 0; --i) {
        if (sorted[i] < 53) order[n++] = sorted[i];
    }
    for (int i = N - 1; i >= 0; --i) {
        if (sorted[i] > 53) order[n++] = sorted[i];
    }
    show("C-LOOK", 53, order, n, 0);
    return 0;
}