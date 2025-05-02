#include <stdio.h>
#include <stdlib.h>

float* gauss_seidel(float **A, float *b, int n, int iterations) {
    float *x = (float *)malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) {
        x[i] = 0;
    }
    for (int k = 0; k < iterations; k++) {
        for (int i = 0; i < n; i++) {
            float sum = b[i];
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sum -= A[i][j] * x[j];
                }
            }
            x[i] = sum / A[i][i];
        }
    }
    return x;
}

int main() {
    int n = 3, iterations = 10;
    float **A = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        A[i] = (float *)malloc(n * sizeof(float));
    }
    A[0][0] = 4; A[0][1] = -1; A[0][2] = 0;
    A[1][0] = -1; A[1][1] = 4; A[1][2] = -1;
    A[2][0] = 0; A[2][1] = -1; A[2][2] = 4;
    float b[3] = {15, 10, 10};
    float *x = gauss_seidel(A, b, n, iterations);
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %f\n", i, x[i]);
    }
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(x);
    return 0;
}
