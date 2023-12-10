#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e6 + 100;
using ll = int64_t;
using PII = std::pair<int, int>;
int n, m, k;
std::array<int, N> nums{};
std::array<int, N> ne{};
char s[N], p[N];

int main() {
    fhj();
    cin >> n >> (p + 1);
    cin >> m >> (s + 1);
    for (int i = 2, j = 0; i <= n; ++i) {
        while (j && p[i] != p[j + 1]) j = ne[j];
        if (p[i] == p[j + 1]) j++;
        ne[i] = j;
    }
    for (int i = 1, j = 0; i <= m; ++i) {
        while (j && s[i] != p[j + 1]) j = ne[j];
        if (s[i] == p[j + 1]) ++j;
        if (j == n) {
            cout << i - n << " ";
            j = ne[j];
        }
    }
    return 0;
}