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
std::vector<PII> nums{};


int main() {
    fhj();
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int l, r;
        cin >> l >> r;
        nums.emplace_back(l, r);
    }
    std::sort(nums.begin(), nums.end(), [](const PII &lhs, const PII &rhs) {
        return lhs.second == rhs.second ? lhs.first < rhs.first: lhs.second < rhs.second;
    });
    int cnt = 0, curr = std::numeric_limits<int>::min();
    for (const auto& p : nums) {
        if ( curr >= p.first && curr <= p.second){
            continue;
        } else {
            ++cnt;
            curr = std::max(curr, p.second);
        }
    }
    std::cout << cnt << endl;
    return 0;
}