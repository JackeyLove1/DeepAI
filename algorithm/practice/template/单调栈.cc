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
std::array<int, N> nums{};

int main() {
    fhj();
    cin >> n;
    std::vector<int> q;
    for (int i = 0; i < n; ++i){
        int x;
        cin >> x;
        while (!q.empty() && q.back() >= x){
            q.pop_back();
        }
        if (q.empty()){
            q.push_back(x);
            cout << -1 << " ";
            continue;
        }
        cout << q.back() << " ";
        q.push_back(x);
    }
    return 0;
}