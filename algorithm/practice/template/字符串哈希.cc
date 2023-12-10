#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e6 + 100;
using ll = int64_t;
using ull = uint64_t;
using PII = std::pair<int, int>;
constexpr uint64_t
P = 131;
char s[N];
ull h[N], p[N];
int n, m;

inline ull get(int l, int r) {
    return h[r] - h[l - 1] * p[r - l + 1];
}

int main() {
    fhj();
    cin >> n >> m;
    cin >> (s + 1);
    p[0] = 1;
    for (int i = 1; i <= n; ++i) {
        h[i] = h[i - 1] * P + s[i];
        p[i] = p[i - 1] * P;
    }
    while (m--) {
        int l0, r0, l1, r1;
        cin >> l0 >> r0 >> l1 >> r1;
        if (get(l0, r0) == get(l1, r1)) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }
    return 0;
}