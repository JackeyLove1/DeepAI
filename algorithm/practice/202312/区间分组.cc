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

class Range {
public:
    int l_, r_;

    explicit Range(int l, int r) : l_(l), r_(r) {};

    bool operator<(const Range &rhs) const {
        return l_ < rhs.l_;
    }
};

std::vector<Range> ranges{};

int main() {
    fhj();
    cin >> n;
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap{};
    for (int i = 0; i < n; ++i) {
        int l, r;
        cin >> l >> r;
        nums.emplace_back(l, r);
    }
    std::sort(nums.begin(), nums.end(), [](const PII &lhs, const PII &rhs) {
        return lhs.first == rhs.first ? lhs.second < rhs.second : lhs.first < rhs.first;
    });

    for (const auto &p: nums) {
        if (heap.empty()){
            heap.push(p.second);
            continue;
        }
        if (heap.top() < p.first){
            heap.pop();
            heap.push(p.second);
            continue;
        }
        heap.push(p.second);
    }
    cout << heap.size() << endl;
    return 0;
}