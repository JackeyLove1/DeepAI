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
int n, m;
std::array<int, N> nums{};

int main() {
    fhj();
    cin >> n;
    for(int i = 0; i < n; ++i) cin >> nums[i];
    std::unordered_set<int> s;
    int res = 1;
    int l = 0, r = 0;
    while (r < n){
        if (!s.count(nums[r])) {
            s.insert(nums[r]);
            ++r;
        }else{
            s.erase(nums[l]);
            ++l;
        }
        res = std::max(res, r - l);
    }
    std::cout << res << endl;
    return 0;
}