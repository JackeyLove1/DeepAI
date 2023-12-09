#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e6 + 10;
int n, m;
int l, r;
std::array<int, N> nums{}, presum{};

int main() {
    fhj();
    std::cin >> n >> m;
    for (int i = 0; i < n; i++) {
        std::cin >> nums[i];
        presum[i + 1] = presum[i] + nums[i];
    }
    while (m--) {
        std::cin >> l >> r;
        std::cout << presum[r] - presum[l - 1] << std::endl;
    }
    return 0;
}