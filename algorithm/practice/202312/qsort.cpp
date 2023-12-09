#include <bits/stdc++.h>
#include <array>
using namespace std;

static void fhj() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

const int N = 1e6 + 10;
std::array<int, N> nums{};
int n;

void qSort(int l, int r) {
    if (l >= r) {
        return;
    }
    int i = l, j = r;
    int mid = nums[(l + r) / 2];
    while (i <= j) {
        while (nums[i] < mid) ++i;
        while (nums[j] > mid) --j;
        if (i <= j) {
            std::swap(nums[i], nums[j]);
            ++i;
            --j;
        }
    }
    if(l < j) qSort(l, j);
    if(i < r) qSort(i, r);
}

int main() {
    fhj();
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    qSort(0, n-1);
    for (int i = 0; i < n; ++i){
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
}