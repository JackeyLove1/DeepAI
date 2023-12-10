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
int h[N], w[N], e[N], ne[N], idx;
int dist[N];
bool st[N];

inline void add(int a, int b, int c){
    e[idx] = b,w[idx] = c, ne[idx] = h[a], h[a] = idx++;
}

inline int spfa(){
    std::queue<int> q;
    q.push(1);
    st[1] = true;
    dist[1] = 0;
    while (!q.empty()){
        auto t = q.front();
        q.pop();
        st[t] = false;
        for (int i = h[t]; i!= -1; i = ne[i]) {
            int j = e[i];
            if (dist[t] + w[i] < dist[j]){
                dist[j] = dist[t] + w[i];
                if (!st[j]) q.push(j), st[j] = true;
            }
        }
    }
    return dist[n];
}
int main() {
    fhj();
    cin >> n >> m;
    memset(h, -1, sizeof h);
    memset(dist, 0x3f, sizeof dist);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        add(a, b, c);
    }
    if (spfa() == INF) cout << "impossible" << endl;
    else cout << dist[n] << endl;
    return 0;
}