#include <bits/stdc++.h>

static inline void fhj() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

using std::cin, std::cout, std::endl;
static constexpr int N = 1e6 + 100;

int longestSemiRepetitiveSubstring(std::string s) {
    if (s.length() == 1) return 1;
    std::array<int, 100> f{}, g{};
    int res = 2;
    if (s[0] == s[1]) f[0] = 0, f[1] = 0, g[1] = 2;
    else f[0] = 1, f[1] = 2, g[1] = 0;
    for (int i = 2; i < s.size(); ++i) {
        if (s[i] != s[i - 1]) {
            f[i] = std::max(f[i - 1] + 1, 2);
            if (s[i - 1] == s[i - 2]) {
                g[i] = g[i - 1] + 1;
            } else {
                g[i] = std::max(g[i - 2] + 2, g[i - 1] + 1);
            }
        } else {
            f[i] = 0;
            if (s[i - 1] == s[i - 2]) {
                g[i] = f[i - 1] + 1;
            } else {
                g[i] = std::max(f[i - 2] + 2, f[i - 1] + 1);
            }
        }
        res = std::max({res, g[i], f[i]});
    }
    /*
    for (int i = 0; i < s.size(); ++i) {
        std::cout << f[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < s.size(); ++i) {
        std::cout << g[i] << " ";
    }
    */
    return res;
}


int main() {
    cout << longestSemiRepetitiveSubstring("52233") << endl;
    cout << longestSemiRepetitiveSubstring("5294") << endl;
    cout << longestSemiRepetitiveSubstring("11111111") << endl;
    cout << longestSemiRepetitiveSubstring("0001") << endl;
    cout << longestSemiRepetitiveSubstring("0111") << endl;
}