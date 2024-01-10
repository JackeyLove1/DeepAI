#include <cstdio>
#include <iostream>
__global__ void hello_from_gpu() {
    printf("Hello World from the the GPU\n");
    printf("How are you ?\n");
}


int main(void) {
    hello_from_gpu<<<1, 1>>>();
    cudaDeviceSynchronize();
    std::cout << "Hello, World!" << "\n";
    return 0;
}
