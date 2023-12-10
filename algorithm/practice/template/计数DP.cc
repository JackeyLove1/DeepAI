#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 3000;
static constexpr int INF = 0x3f3f3f3f;
using PII = std::pair<int, int>;
int n, m, k;
int f[N];
/*
一个正整数n可以表示成若干个正整数之和，形如：n=n1+n2+…+nk，其中 n1≥n2≥…≥nk,k≥1
我们将这样的一种表示称为正整数n的一种划分。现在给定一个正整数n，请你求出n共有多少种不同的划分方法
*/
int main() {
    fhj();
    cin >> n;
    const int mod = 1e9 + 7;
    f[0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; ++j) {
            f[j] = (f[j] + f[j - i]) % mod;
        }
    }
    cout << f[n] << endl;
    return 0;
}