#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e5 + 100;
using ll = int64_t;
int n, m;
std::array<int, N> a{}, b{};

int main() {
    fhj();
    cin >> n >> m;
    for(int i = 0 ; i < n; ++i) cin >> a[i];
    for(int i = 0 ; i < m; ++i) cin >> b[i];
    int i = 0, j = 0;
    while (i < n && j < m){
        if (a[i] == b[j]) ++i, ++j;
        else ++j;
    }
    if (i == n){
        std::cout << "Yes" << std::endl;
    }else {
        std::cout << "No" << std::endl;
    }
    return 0;
}