#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

constexpr int N = 1e6 + 10;
std::array<int, N> nums{}, tmp{};
int n;

uint64_t mergeSort(int l, int r) {
    if (l >= r) return 0;
    int mid = l + r >> 1;
    uint64_t res = 0;
    res += mergeSort(l, mid);
    res += mergeSort(mid + 1, r);
    int k = 0, i = l, j = mid + 1;
    while (i <= mid && j <= r) {
        if (nums[i] <= nums[j]) tmp[k++] = nums[i++];
        else tmp[k++] = nums[j++], res += mid - i + 1;
    }
    while (i <= mid) tmp[k++] = nums[i++];
    while (j <= r) tmp[k++] = nums[j++];
    for (int i = l, k = 0; i <= r; ++i, ++k) {
        nums[i] = tmp[k];
    }
    return res;
}

int main() {
    fhj();
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    std::cout << mergeSort(0, n - 1) << std::endl;
    return 0;
}