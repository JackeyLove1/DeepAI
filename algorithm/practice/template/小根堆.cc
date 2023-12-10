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
    uint64_t res = 0;
    cin >> n;
    std::priority_queue<int, std::vector<int>, std::greater<>> heap{};
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        heap.push(x);
    }
    while (heap.size() >= 2) {
        auto a = heap.top();
        heap.pop();
        auto b = heap.top();
        heap.pop();
        res += a + b;
        heap.push(a + b);
    }
    cout << res << endl;
    return 0;
}