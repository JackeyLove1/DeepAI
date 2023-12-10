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
std::vector<int> nums;

int main() {
    fhj();
    cin >> n;
    for (int i = 0; i < n; ++i){
        int x ;
        cin >> x;
        nums.push_back(x);
    }
    std::sort(nums.begin(), nums.end());
    uint64_t res = 0;
    int mid = nums[n >> 1];
    for (int i = 0; i < n; ++i){
        res += std::abs(nums[i] - mid);
    }
    cout << res << endl;
    return 0;
}