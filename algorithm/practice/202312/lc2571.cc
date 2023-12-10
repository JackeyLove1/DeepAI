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

int minOperations(int n) {
    int cnt = 0;
    while (n) {
        if (n & 1) {
            if ((n >> 1) & 1) n += 1, ++cnt;
            else n -= 1, ++cnt;
        }
        n >>= 1;
    }
    return cnt;
}

int minOperations1(int n) {
    std::vector<int> nums;
    const int N = 2e5 + 100;
    int f[N];
    memset(f, 0x3f, sizeof f);
    f[0] = 0;
    int num = 1;
    for (; num < n; num *= 2) {
        nums.push_back(num);
    }
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = nums[i]; j <= num; ++j) {
            f[j] = std::min(f[j], f[j - nums[i]] + 1);
        }
    }
    return f[n];
}

int main() {
    cout << minOperations(27) << " expected:" << 3 << endl;
    cout << minOperations(39) << " expected:" << 3 << endl;
}