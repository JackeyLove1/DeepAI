#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e5 + 100;
using PII = std::pair<int, int>;
int n, m;
int h[N], e[N], ne[N], idx;
int d[N];
int res[N];

inline void add(int a, int b){
    e[idx] = b, ne[idx] = h[a], h[a] = idx++;
}

bool topsort(){
    std::queue<int>q;
    int cnt = 0;
    for (int i = 1; i <= n; ++i){
        if(!d[i]){
            q.push(i);
        }
    }
    while (!q.empty()){
        auto t = q.front();
        q.pop();
        res[cnt++] = t;
        for (int i = h[t]; i != -1; i = ne[i]){
            int j = e[i];
            if(--d[j] == 0){
                q.push(j);
            }
        }
    }
    return cnt == n;
}

int main() {
    std::memset(h, -1, sizeof h);
    fhj();
    cin >> n >> m;
    for (int i = 0; i < m; ++i){
        int a, b;
        cin >> a >> b;
        add(a, b);
        d[b]++;
    }
    if (topsort()){
        for(int i = 0 ;i < n; ++i) cout << res[i] <<" ";
    }else {
        cout << -1 << endl;
    }
    return 0;
}