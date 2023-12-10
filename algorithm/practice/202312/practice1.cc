#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e6 + 100;
int n, m;
using ll = int64_t;
namespace treeArray {
    ll tr[N];

    inline int lowbit(int x) {
        return x & (-x);
    }

    inline void add(int x, int c) {
        for (int i = x; i <= n; i += lowbit(x)) {
            tr[i] += c;
        }
    }

    inline ll query(int x) {
        ll res = 0;
        for (int i = x; i >= 0; i -= lowbit(x)) {
            res += tr[i];
        }
        return res;
    }
}


namespace segmentTree {
    int w[N];
    struct Node {
        int l, r;
        ll sum, add;
    } tr[N * 4];

    void pushup(int u) {
        tr[u].sum = tr[u << 1].sum + tr[u << 1 | 1].sum;
    }

    void pushdown(int u) {
        auto &root = tr[u];
        if (tr[u].add) {
            auto &left = tr[u << 1], &right = tr[u << 1 | 1];
            left.add += root.add;
            left.sum += static_cast<ll>((left.r - left.l + 1)) * root.add;
            right.add += root.add;
            right.sum += static_cast<ll>((right.r - right.l + 1)) * root.add;
            root.add = 0;
        }
    }

    void build(int u, int l, int r) {
        if (l == r) tr[u] = {l, r, w[l], 0};
        else {
            tr[u] = {l, r, 0, 0};
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }

    void modify(int u, int l, int r, ll d) {
        if (l <= tr[u].l && tr[u].r <= r) {
            tr[u].add += d;
            tr[u].sum += (tr[u].r - tr[u].l + 1) * d;
        } else {
            pushdown(u);
            int mid = tr[u].l + tr[u].r >> 1;
            if (l <= mid) modify(u << 1, l, r, d);
            if (r > mid) modify(u << 1 | 1, l, r, d);
            pushup(u);
        }
    }

    ll query(int u, int l, int r) {
        if (tr[u].l >= l && tr[u].r <= r) return tr[u].sum;
        pushdown(u);
        int mid = tr[u].l + tr[u].r >> 1;
        ll sum = 0;
        if (l <= mid) sum += query(u << 1, l, r);
        if (r > mid) sum += query(u << 1 | 1, l, r);
        return sum;
    }
}

namespace calculateDP {
    const int N = 15;
    int a[N], len;
    ll dp[N][N];

    ll dfs(int pos, int pre, bool lead, bool limit) {
        if (pos > len) return 1;
        if (!limit && dp[pos][pre] != -1)
            return 0;
    }

    ll solve(ll x) {
        len = 0;
        return 0;
    }

    ll query(ll a, ll b) {
        return solve(b) - solve(a - 1);
    }
}

int main() {
    using key = std::tuple<std::string, int, int>;
    std::map<key, int> m;
    m[{"1", 2, 3}] = 1;
    cout << m[{"1", 2, 3}] << endl;
}