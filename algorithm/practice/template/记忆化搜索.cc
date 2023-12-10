// 单纯限定区域的记忆化搜索
// https://www.acwing.com/problem/content/903/
#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 330;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
using ll = long long;
int n, m, k;
int g[N][N];
std::array <std::array<int, N>, N> f{};
const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};

int solve(int x, int y) {
    int &v = f[x][y];
    if (v != -1) return v;
    v = 1;
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 1 && nx <= n && ny >= 1 && ny <= m && g[x][y] > g[nx][ny]) {
            v = std::max(v, 1 + solve(nx, ny));
        }
    }
    return v;
}

int main() {
    fhj();
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cin >> g[i][j];
        }
    }
    for (int i = 0; i <= n; ++i){
        f[i].fill(-1);
    }
    int res = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            res = std::max(res, solve(i, j));
        }
    }
    cout << res << endl;
    return 0;
}

