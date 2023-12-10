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
int p[N];

struct Edge {
    int from, to, cost;

    bool operator<(const Edge &w) const {
        return cost < w.cost;
    }
} edge[N];


int find(int x) {
    if (p[x] != x) p[x] = find(p[x]);
    return p[x];
}

void merge(int a, int b) {
    p[find(a)] = p[find(b)];
}

int kruskal() {
    for (int i = 1; i <= n; ++i) p[i] = i;
    std::sort(edge, edge + m);
    int res = 0, cnt = 0;
    for (int i = 0; i < m; ++i) {
        int x = edge[i].from, y = edge[i].to, c = edge[i].cost;;
        if (find(x) != find(y)) {
            merge(x, y);
            res += c;
            if (++cnt == n - 1) break;
        }
    }
    if (cnt < n - 1) return -1;
    return res;
}

int main() {
    fhj();
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        edge[i] = {a, b, c};
    }
    auto result = kruskal();
    if (result == -1) cout << "impossible" << endl;
    else cout << result << endl;
    return 0;
}