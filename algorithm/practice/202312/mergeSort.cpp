#include <bits/stdc++.h>
#include <array>

using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

const int N = 1e6 + 10;
std::array<int, N> nums{}, tmp{};
int n;

void mergeSort(int l, int r) {
    if (l >= r) return;
    int mid = l + r >> 1;
    mergeSort(l, mid);
    mergeSort(mid + 1, r);
    int k = 0, i = l, j = mid + 1;
    while (i <= mid && j <= r) {
        if (nums[i] <= nums[j]) tmp[k++] = nums[i++];
        else tmp[k++] = nums[j++];
    }
    while (i <= mid) tmp[k++] = nums[i++];
    while (j <= r) tmp[k++] = nums[j++];
    for (int i = l, k = 0; i <= r; i++, k++) {
        nums[i] = tmp[k];
    }
}

int main() {
    fhj();
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    mergeSort(0, n - 1);
    for (int i = 0; i < n; ++i) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
}