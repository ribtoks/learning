#include <iostream>

#define MAXN 10000
#define INF 1000000000

int main () {
    int d[MAXN + 1];
    int p[MAXN + 1];
    int arr[MAXN];

    int n;
    std::cin >> n;
    int m = n, i = 0, a;
    while (m--) {
        std::cin >> a;
        arr[i++] = a;
    }

    // LIS - Longest Increasing Subsequence
    // d[i] - smallest element which is last in LIS of length i
    d[0] = -INF; p[0] = -1;
    for (int i = 1; i <= n; ++i) { d[i] = INF; p[i] = -1; }

    // try all elements as next element of each subsequence
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= n; ++j) {
            // can be next to subsequence of length (j - 1)
            if (d[j - 1] < a[i] &&
                // minimal element of those
                a[i] < d[j]) {
                //
                d[j] = a[i];
                p[j] = i;
            }
        }
    }

    int pmax = -1, dmax = -1;

    for (int i = 0; i < n; ++i) {
        if (d[i] > dmax) {
            dmax = d[i];
            pmax = i;
        }
    }

    int curr = pmax;
    while (curr != -1) {
        std::cout << arr[curr] << " ";
        curr = p[curr];
    }

    return 0;
}
