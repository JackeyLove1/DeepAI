#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
// static constexpr int N = 330;
static constexpr int INF = 0x3f3f3f3f;
using ll = long long;
int n, m, k;
using namespace std;

const int N = 3 * 10e4 + 100;
using PII = std::pair<int, int>;
std::map<PII, bool> f;

bool solve(const string &s, int l, int r) {
    if (l >= r || r >= s.size()) return false;
    if (l + 1 == r) {
        if (s[l] == '(' && s[r] == ')') {
            return true;
        } else return false;
    }
    if (f.find({l, r}) != f.end()) {
        return f[{l, r}];
    }
    f[{l, r}] = false;
    bool &ret = f[{l, r}];
    if (s[l] == '(' && s[r] == ')') {
        ret = solve(s, l + 1, r - 1);
    }
    for (int k = l + 1; k < r; ++k) {
        ret = ret || (solve(s, l, k) && solve(s, k + 1, r));
    }
    return f[{l, r}];
}

int longestValidParentheses(string s) {
    f.clear();
    int res = 0;
    for (int i = 0; i < s.size(); ++i) {
        for (int j = i + 1; j < s.size(); ++j) {
            if (solve(s, i, j)) {
                res = std::max(res, j - i + 1);
                // std::cout << "i:" << i << " j:" << j << " res:" << res << endl;
            }
        }
    }
    return res;
}

int main() {
    // std::cout << longestValidParentheses("(()") << " res:2" << endl;
    std::cout << longestValidParentheses(")()())") << " res:4" << endl;
    for (const auto &[k, v]: f) {
        cout << "l:" << k.first << " r:" << k.second << " v:" << v << endl;
    }
}