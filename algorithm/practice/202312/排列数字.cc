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
int n;
std::array<bool, N> st{};
std::array<int, N> path{};

void dfs(int index) {
    if (index == n){
        for (int i = 0; i < n; ++i){
            cout << path[i] <<" ";
        }
        cout << endl;
        return;
    }
    for (int i = 1; i <= n; ++i){
        if (!st[i]){
            st[i] = true;
            path[index] = i;
            dfs(index + 1);
            st[i] = false;
        }
    }
}

int main(){
    cin >> n;
    dfs(0);
}