#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e6 + 10;
using ll = int64_t;

inline ll qsm(ll a, ll b, ll mod) {
    ll res = 1;
    while (b) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

inline ll qsc(ll a, ll b, ll mod){
    ll res = 0;
    while (b){
        if (b & 1) res = (res + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return res;
}