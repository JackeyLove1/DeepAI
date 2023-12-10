#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e4 + 100;
using PII = std::pair<int, int>;
int n, m;

int g[N][N], d[N][N];

int bfs() {
    std::queue <PII> q;
    memset(d, -1, sizeof d);
    d[0][0] = 0;
    q.push({0, 0});
    const int dx[] = {1, 0, -1, 0};
    const int dy[] = {0, 1, 0, -1};
    while (!q.empty()) {
        auto t = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int x = dx[i] + t.first, y = dy[i] + t.second;
            if (x >= 0 && x < n && y >= 0 && y < m && d[x][y] == -1 && g[x][y] == 0) {
                q.push({x, y});
                d[x][y] = d[t.first][t.second] + 1;
            }
        }
    }
    return d[n - 1][m - 1];
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> g[i][j];
        }
    }

    cout << bfs() << endl;
    return 0;
}