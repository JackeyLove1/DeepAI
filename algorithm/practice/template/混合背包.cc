// https://www.acwing.com/problem/content/7/
// 采用01二进制的方法将分组背包转换为01背包
const int N = 2e4 + 100;
int f[N];

struct good {
    int v, w, s;
};

int main() {
    int n, m;
    cin >> n >> m;
    vector<good> goods;
    for (int i = 0; i < n; i++) {
        int v, w, s;
        cin >> v >> w >> s;
        if (s > 0) {
            for (int k = 1; k <= s; k *= 2) {
                s -= k;
                goods.push_back({k * v, k * w, 0});
            }
            if (s) goods.push_back({s * v, s * w, 0});
        } else if (s == -1) {
            goods.push_back({v, w, 0});
        } else {
            goods.push_back({v, w, 1});
        }
    }
    for (auto &g : goods) {
        if (!g.s) {
            for (int j = m; j >= g.v; j--) {
                f[j] = max(f[j], f[j - g.v] + g.w);
            }
        } else {
            for (int j = g.v; j <= m; j++) {
                f[j] = max(f[j], f[j - g.v] + g.w);
            }
        }
    }
    cout << f[m] << endl;
    return 0;
}

// 二维费用背包问题
// https://www.acwing.com/problem/content/1022/
{
    cin >> n >> m >> t;
    for (int i = 1; i <= t; ++ i) cin >> v1[i] >> v2[i] >> w[i];

    memset(f, 0x3f, sizeof f); //求最小值要把除初始状态以外的所有状态初始化为+∞
    f[0][0] = 0; //这里我们把所有j,k小于0的初始状态都合并到f[0][0][0]中来转移,也就是下面的max操作
    for (int i = 1; i <= t; ++ i)
    {
        for (int j = n; j >= 0; -- j)
        {
            for (int k = m; k >= 0; -- k)
            {
                f[j][k] = min(f[j][k], f[max(j - v1[i], 0)][max(k - v2[i], 0)] + w[i]);
            }
        }
    }
    cout << f[n][m] << endl;
    return 0;
}
