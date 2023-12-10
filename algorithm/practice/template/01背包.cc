#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e6 + 100;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
int n, m;
int f[N];

int main() {
    fhj();
    cin >> n >> m;
    int res = 0;
    for (int i = 0; i < n; ++i) {
        int v, w;
        cin >> v >> w;
        for (int j = m; j >= v; --j) {
            f[j] = std::max(f[j], f[j - v] + w);
            res = std::max(res, f[j]);
        }
    }
    cout << res << endl;
    return 0;
}