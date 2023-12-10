#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 3000;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
int n, m, k;
int nums[N], s[N], f[N][N];

int main() {
    fhj();
    cin >> n;
    memset(f, 0x3f, sizeof f);
    for (int i = 1; i <= n; ++i) cin >> nums[i];
    for (int i = 1; i <= n; i++) s[i] = s[i - 1] + nums[i];
    for (int i = 1; i <= n; ++i) f[i][i] = 0;
    for (int len = 2; len <= n; ++len) {
        for (int l = 1; l + len - 1<= n; ++l) {
            int r = l + len - 1;
            for (int k = l; k < r; ++k) {
                f[l][r] = std::min(f[l][r], f[l][k] + f[k + 1][r] + s[r] - s[l - 1]);
            }
        }
    }
    cout << f[1][n] << endl;
    return 0;
}