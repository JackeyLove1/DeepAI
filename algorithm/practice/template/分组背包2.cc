#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 300;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
int n, m, k;
int f[N], v[N][N], w[N][N], s[N];

int main() {
    fhj();
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> s[i];
        for (int j = 0; j < s[i]; ++j) {
            cin >> v[i][j] >> w[i][j];
        }
    }
    int res = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = m; j >= 0; --j) {
            for (int k = 0; k < s[i]; ++k) {
                if (j >= v[i][k]) {
                    f[j] = std::max(f[j], f[j - v[i][k]] + w[i][k]);
                    res = std::max(res, f[j]);
                }
            }
        }
    }
    cout << res << endl;
    return 0;
}