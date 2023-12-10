inline int solve(std::string &a, std::string &b) {
    int n = a.size(), m = b.size();
    memset(f, 0x3f, sizeof f);
    for (int i = 0; i <= n; ++i) f[i][0] = i;
    for (int i = 0; i <= m; ++i) f[0][i] = i;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            f[i][j] = std::min(f[i - 1][j], f[i][j - 1]) + 1;
            f[i][j] = std::min(f[i][j], f[i - 1][j - 1] + (a[i] != b[j]));
        }
    }
    return f[n][m];
}