#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e6 + 10;
using ll = int64_t;
int n, m, t;

std::array<ll, N> d;

void insert(int l, int r, int c) {
    d[l] += c;
    d[r + 1] -= c;
}

int main() {
    fhj();
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        cin >> t;
        insert(i, i, t);
    }
    int l, r, c;
    while (m--) {
        cin >> l >> r >> c;
        insert(l, r, c);
    }
    int sum = d[0];
    for (int i = 1; i <= n; ++i) {
        sum = sum + d[i];
        cout << sum << " ";
    }
    return 0;
}