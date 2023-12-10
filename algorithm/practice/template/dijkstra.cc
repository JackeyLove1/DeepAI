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

inline void add(int x, int y, int z){
    e[idx] = y, w[idx] = z, ne[idx] = h[x], h[x] = idx++;
}


inline int  dijkstra(){
    dist[1] =0;
    std::priority_queue<PII, std::vector<PII>, std::greater<>> q;
    q.push({0, 1});
    while (!q.empty()){
        auto [_, point] = q.top();
        q.pop();
        if (st[point]) continue;
        st[point] = true;
        for (int i = h[point]; i != -1; i = ne[i]){
            int j = e[i];
            if (dist[j] > dist[point] + w[i]){
                dist[j] = dist[point] + w[i];
                q.push({dist[j], j});
            }
        }
    }
    if (dist[n] == INF) return -1;
    return dist[n];
}


int main(){
    fhj();
    memset(h, -1, sizeof h);
    memset(dist, 0x3f, sizeof dist);
    memset(st, false, sizeof st);
    cin >> n >> m;
    for (int i = 0; i < m; ++i){
        int x, y, z;
        cin >> x >> y >> z;
        add(x, y, z);
    }
    cout << dijkstra() << endl;
    return 0;
}