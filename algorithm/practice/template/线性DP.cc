// 方格取数
// https://www.acwing.com/problem/content/1029/
const int N = 22;
int w[N][N];
int f[N][N][N];

int main() {
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    int a, b, c;
    while (cin >> a >> b >> c, a || b || c) w[a][b] = c;

    for (int k = 2; k <= n * 2; k++) {
        for (int i1 = 1; i1 <= n; i1++) {
            for (int i2 = 1; i2 <= n; i2++) {
                int j1 = k - i1, j2 = k - i2;
                if (j1 >= 1 && j1 <= n && j2 >= 1 && j2 <= n) {
                    int &x = f[k][i1][i2];
                    int t = w[i1][j1];
                    if (i1 != i2) t += w[i2][j2];
                    x = max(x, f[k - 1][i1 - 1][i2 - 1] + t);
                    x = max(x, f[k - 1][i1 - 1][i2] + t);
                    x = max(x, f[k - 1][i1][i2 - 1] + t);
                    x = max(x, f[k - 1][i1][i2] + t);
                }
            }
        }
    }

    cout << f[2 * n][n][n] << endl;
}

// 最长公共上升子序列
// https://www.acwing.com/problem/content/274/
for (int i = 1; i <= n; i ++ )
{
    for (int j = 1; j <= n; j ++ )
    {
        f[i][j] = f[i - 1][j];
        if (a[i] == b[j])
        {
            int maxv = 1;
            for (int k = 1; k < j; k ++ )
                if (a[i] > b[k])
                    maxv = max(maxv, f[i - 1][k] + 1);
            f[i][j] = max(f[i][j], maxv);
        }
    }
}