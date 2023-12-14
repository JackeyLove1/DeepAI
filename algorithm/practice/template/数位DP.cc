// 数字游戏
// https://www.acwing.com/problem/content/1084/
/*
某人命名了一种不降数，这种数字必须满足从左到右各位数字呈非下降关系，如 123，446。
现在大家决定玩一个游戏，指定一个整数闭区间 [a,b]，问这个区间内有多少个不降数。
*/
static int f[11][10][3];
int a[20];

int dfs(int pos, int pre, bool limit) {
    if (pos == -1) return 1;
    int &v = f[pos][pre][limit];
    if (v != -1) return v;
    v = 0;
    int up = limit ? a[pos] : 9;
    for (int i = pre; i <= up; ++i) {
        v += dfs(pos - 1, i, limit && i == a[pos]);
    }
    return v;
}

int solve(int x) {
    if (!x) return 1;
    int pos = 0;
    memset(f, -1, sizeof f);
    while (x) {
        a[pos++] = x % 10;
        x /= 10;
    }
    return dfs(pos - 1, 0, true);
}


int main() {
    fhj();
    int l, r;
    while (cin >> l >> r) {
        cout << solve(r) - solve(l - 1) << endl;
    }
    return 0;
}

// Windy数
using ll = long long;
int a[15], len;
ll dp[15][15];//pos, pre,

ll dfs(int pos, int pre, bool lead/*是否有前导零*/, bool limit) {
    if (pos > len) return 1;
    if (!limit && dp[pos][pre] != -1) return dp[pos][pre];
    ll ret = 0;
    int up = limit ? a[len - pos + 1] : 9;//当前位最大数字
    for (int i = 0; i <= up; i++)//从0枚举到最大数字
    {
        if (abs(i - pre) < 2) continue;
        if (lead && i == 0) ret += dfs(pos + 1, -2, 1, limit && i == up);
        else ret += dfs(pos + 1, i, 0, limit && i == up);
    }
    if (!limit && !lead) dp[pos][pre] = ret;
    return ret;
}

ll solve(ll x) {
    len = 0;
    while (x) {
        a[++len] = x % 10;
        x /= 10;
    }
    memset(dp, -1, sizeof dp);
    return dfs(1, -2, true, true);
}

ll l, r;

int main() {
    cin >> l >> r;
    cout << solve(r) - solve(l - 1) << endl;
    return 0;
}