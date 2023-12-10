// 1. 状态兼容型，寻找所有兼容状态然后DP
// https://www.acwing.com/problem/content/293/
#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

// 记录所有状态然后dp
using std::cin, std::cout, std::endl, std::vector;
static constexpr int N = 13, M = 1 << N;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
using ll = long long;
int n, m, k;
ll f[N][M];
bool st[M];
vector<int> state[M];

int main() {
    fhj();
    while (cin >> n >> m, n || m) {
        for (int i = 0; i < (1 << n); ++i) {
            int cnt = 0;
            bool isValid = true;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    if (cnt & 1) {
                        isValid = false;
                        break;
                    }
                    cnt = 0;
                } else {
                    ++cnt;
                }
            }
            if (cnt & 1) isValid = false;
            st[i] = isValid;
        }

        for (int j = 0; j < (1 << n); ++j) {
            state[j].clear();
            for (int k = 0; k < (1 << n); ++k) {
                if ((j & k) == 0 && st[j | k]) {
                    state[j].push_back(k);
                }
            }
        }

        memset(f, 0, sizeof f);
        f[0][0] = 1;
        for (int i = 1; i <= m; ++i) {
            for (int j = 0; j < (1 << n); ++j) {
                for (auto k: state[j]) {
                    f[i][j] += f[i - 1][k];
                }
            }
        }
        cout << f[m][0] << endl;
    }
    return 0;
}

// 2. 使用二进制/N进制表示多个状态的转移
// https://www.acwing.com/problem/content/93/
#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl, std::vector;
static constexpr int N = 21;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
using ll = long long;
int n, m, k;
int w[N][N];
int f[1 << N][N];

int main() {
    fhj();
    cin >> n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> w[i][j];
        }
    }
    memset(f, 0x3f, sizeof f);
    f[0][0] = 0;
    for (int i = 0; i < (1 << n); ++i) {
        for (int j = 0; j < n; ++j) {
            if ((i >> j) & 1) {
                for (int k = 0; k < n; ++k) {
                    if ((i >> k) & 1) {
                        f[i][j] = std::min(f[i][j], f[i - (1 << j)][k] + w[k][j]);
                    }
                }
            }
        }
    }
    cout << f[(1 << n) - 1][n - 1] << endl;
    return 0;
}

//