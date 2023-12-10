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
std::vector<PII> range{};
int n;


int main() {
    fhj();
    cin >> n;
    range.reserve(n);
    for (int i = 0; i < n; ++i) {
        int l, r;
        cin >> l >> r;
        range.emplace_back(l, r);
    }
    std::sort(range.begin(), range.end(), [](const PII &lhs, const PII &rhs) {
        return lhs.first == rhs.first ? lhs.second < rhs.second : lhs.first < rhs.first;
    });
    int cnt = 0, curl = 0, curr = 0;
    for (const auto &p: range) {
        if (cnt == 0) {
            curl = p.first;
            curr = p.second;
            ++cnt;
            continue;
        }
        if (curr < p.first) {
            curl = p.first;
            curr = p.second;
            ++cnt;
            continue;
        }
        curr = std::max(curr, p.second);
    }
    cout << cnt << endl;
    return 0;
}