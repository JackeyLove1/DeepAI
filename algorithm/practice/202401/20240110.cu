#include <cstdio>
#include <iostream>

__global__ void hello_from_gpu() {
    printf("Hello World from the the GPU\n");
    printf("How are you ?\n");
    const auto bid = blockIdx.x;
    const auto tid = threadIdx.x;
    const auto idx = tid + bid * blockDim.x;
    printf("bid:%d, tid:%d, idx:%d\n", bid, tid, idx);
}


int main(void) {
    hello_from_gpu<<<2, 4>>>();
    cudaDeviceSynchronize();
    std::cout << "Hello, World!" << "\n";
    return 0;
}
