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
int n, m, k;
std::array<int, N> nums{};
int son[N][26], cnt[N];
int idx;
char op;
std::string strs;
inline void insert(const std::string &s) {
    int p = 0;
    for (int i = 0; i < s.size(); ++i) {
        int u = s[i] - 'a';
        if (!son[p][u]) son[p][u] = ++idx;
        p = son[p][u];
    }
    ++cnt[p];
}

inline int query(const std::string &s) {
    int p = 0;
    for (int i = 0; i < s.size(); ++i) {
        int u = s[i] - 'a';
        if (!son[p][u]) return 0;
        p = son[p][u];
    }
    return cnt[p];
}

int main() {
    fhj();
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i){
        cin >> op >> strs;
        if (op == 'I'){
            insert(strs);
        }else {
            cout << query(strs) << endl;
        }
    }
    return 0;
}