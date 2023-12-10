// 二分然后check
// https://www.acwing.com/problem/content/description/104/
using namespace std;
const int N = 1e5 + 100;
double cow[N];
double sum[N]; // 前缀和
int n, m;

bool check(double mid) {
    // 计算减去mid后的前缀和
    // 本质做法(sum[i+m] - sum[i])/m >= mid
    // 转化为 sum[i+m] - sum[i] - m * mid >= 0
    for (int i = 1; i <= n; i++) sum[i] = sum[i - 1] + (cow[i] - mid);
    double minv = 0;
    for (int i = 0, j = m; j <= n; i++, j++) {
        minv = min(minv, sum[i]);
        if (sum[j] >= minv) return true; // 有一个满足条件就行
    }
    return false;
}

double bsearch(double l, double r) {
    while (r - l > 1e-5) {
        double mid = (l + r) / 2;
        if (check(mid)) l = mid;
        else r = mid;
    }
    return r; // 往较大处取数
}

int main() {
    cin >> n >> m;
    double l = 0, r = 0;
    for (int i = 1; i <= n; ++i) {
        cin >> cow[i];
        r = max(r, cow[i]);
    }
    int res = int(bsearch(l, r) * 1000);
    cout << res << endl;
    return 0;
}

