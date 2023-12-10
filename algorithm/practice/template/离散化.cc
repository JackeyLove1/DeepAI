#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e6 + 100;
using ll = int64_t;
using PII = std::pair<int, int>;
int n, m;
std::array<int, N> nums{}, presum{};
std::vector<int> alls{};
std::vector<PII> add{}, query{};


int main() {
    fhj();
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int x, c;
        cin >> x >> c;
        alls.push_back(x);
        add.emplace_back(x, c);
    }
    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        query.emplace_back(l, r);
        alls.push_back(l);
        alls.push_back(r);
    }
    sort(alls.begin(), alls.end());
    alls.erase(unique(alls.begin(), alls.end()), alls.end());

    auto find = [&](int x) {
        return lower_bound(alls.begin(), alls.end(), x) - alls.begin() + 1;
    };

    for (const auto &p: add) {
        auto index = find(p.first);
        nums[index] += p.second;
    }
    for (int i = 1; i <= alls.size(); ++i) {
        presum[i] = nums[i] + presum[i - 1];
    }
    for (const auto &p: query) {
        auto l = find(p.first), r = find(p.second);
        cout << presum[r] - presum[l - 1] << endl;
    }
    return 0;
}
/*
sort(alls.begin(), alls.end());
    alls.erase(unique(alls.begin(), alls.end()), alls.end());
    std::unordered_map<int, int> indexs;
    for (int i = 0; i < alls.size(); ++i) {
        indexs[alls[i]] = i;
    }
    auto find = [&](int x) {
        // return lower_bound(alls.begin(), alls.end(), x) - alls.begin() + 1;
        return indexs[x];
    };

    for (const auto &p: add) {
        auto index = find(p.first);
        nums[index] += p.second;
    }
    for (int i = 0; i <= alls.size(); ++i) {
        presum[i + 1] = nums[i] + presum[i];
    }
    for (const auto &p: query) {
        auto l = find(p.first), r = find(p.second);
        cout << presum[r + 1] - presum[l] << endl;
    }
*/