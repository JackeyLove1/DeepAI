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

int main() {
    fhj();
    uint64_t a, b, p;
    cin >> a >> b >> p;
    uint64_t res = 0;
    while (b) {
        if (b & 1) res = (res + a) % p;
        a  = (a * 2) % p;
        b >>= 1;
    }
    cout << res << endl;
    return 0;
}