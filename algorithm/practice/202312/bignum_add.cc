#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e6 + 10;
std::string a, b;
int n, q;

int main() {
    fhj();
    std::cin >> a >> b;
    int add = 0;
    int al = a.length(), bl = b.length();
    std::vector<int> res;
    res.reserve(std::max(al, bl) + 10);
    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());
    int idx = 0;
    while (idx < al && idx < bl){
        int num1 = a[idx] - '0', num2 = b[idx] - '0';
        int sum = num1 + num2 + add;
        res.push_back(sum % 10);
        add = sum / 10;
        ++idx;
    }
    while (idx < al) {
        int num = a[idx] - '0';
        int sum = num + add;
        res.push_back(sum % 10);
        add = sum / 10;
        ++idx;
    }
    while (idx < bl) {
        int num = b[idx] - '0';
        int sum = num + add;
        res.push_back(sum % 10);
        add = sum / 10;
        ++idx;
    }
    if (add) res.push_back(add);
    std::reverse(res.begin(), res.end());
    for (int x : res) cout << x;
    return 0;
}