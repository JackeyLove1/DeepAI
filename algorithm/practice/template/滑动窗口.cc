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
int n, m, k;
std::array<int, N> nums{};

int main() {
    fhj();
    cin >> n >> k;
    std::map<int, int> freqs{};
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    std::vector<int> mins, maxs;
    mins.reserve(n), maxs.reserve(n);
    for (int i = 0; i < k; ++i) {
        freqs[nums[i]]++;
    }
    mins.push_back(freqs.begin()->first);
    maxs.push_back((--freqs.end())->first);
    for (int i = 1, j = k; j < n; ++i, ++j) {
        freqs[nums[j]]++;
        if (--freqs[nums[i - 1]] == 0) {
            freqs.erase(nums[i - 1]);
        }
        mins.push_back(freqs.begin()->first);
        maxs.push_back((--freqs.end())->first);
    }
    for (int i = 0; i + k <= n; ++i) {
        cout << mins[i] << " ";
    }
    cout << endl;
    for (int i = 0; i + k <= n; ++i) {
        cout << maxs[i] << " ";
    }
    return 0;
}