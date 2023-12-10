#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e6 + 100;
using ll = int64_t;
using PII = std::pair<int, int>;
int n, m;
std::string op;
std::array<int, N> p, cnt;

int get(int x) {
    if (p[x] != x) p[x] = get(p[x]);
    return p[x];
}

inline void merge(int a, int b) {
    int fa = get(a), fb = get(b);
    if (fa != fb){
        p[fa] = fb;
        cnt[fb] += cnt[fa];
        cnt[fa] = 0;
    }
}

int main() {
    fhj();
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) p[i] = i, cnt[i] = 1;
    while (m--) {
        cin >> op;
        int a, b;
        if (op == "C") {
            cin >> a >> b;
            if (a == b) continue;
            merge(a, b);
        }
        if (op == "Q1") {
            cin >> a >> b;
            int fa = get(a), fb = get(b);
            if (fa == fb) {
                cout << "Yes" << endl;
            }else {
                cout <<"No" << endl;
            }
        }
        if (op == "Q2") {
            cin >> a;
            cout << cnt[get(a)] << endl;
        }
    }
    return 0;
}