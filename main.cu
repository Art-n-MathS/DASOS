#include <iostream>
#include <math.h>
#include "Maps/MapsManager.h"

#include "bilLib/Las1_3_handler.h"
#include "MarchingCubes/MarchingCubes.h"

__global__ // This keyword means the code runs on the GPU.
void add(int n, float *x, float *y)
    {
    printf("Hello World!\n");
    // At each index, add x to y.
    for (int i = 0; i < n; i++)
        {
        y[i] = x[i] + y[i];
        }
    }

int main(void)
    {
    int N = 100;
    float *x, *y;

    // Allocate Unified Memory – accessible from CPU or GPU
    cudaMallocManaged(&x, N*sizeof(float));
    cudaMallocManaged(&y, N*sizeof(float));

    // Initialize our x and y arrays with some floats.
    for (int i = 0; i < N; i++)
        {
        x[i] = 1.0f;
        y[i] = 2.0f;
        }

    // Run the function on using the GPU.
    add<<<1, 1>>>(N, x, y); // Notice the brackets.

    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        {
        maxError = fmax(maxError, fabs(y[i]-3.0f));
        }
    std::cout << "Max error: " << maxError << std::endl;

    // Free memory
    cudaFree(x);
    cudaFree(y);

    return 0;
    }
