#include <stdio.h>
#include <stdlib.h>

float linear_interpolation(float x, float x1, float y1, float x2, float y2) {
    return y2 + (y2 - y1) / (x2 - x1) * (x - x1);
}

float* get_norder(int n, float** pts, int numPoints) {
    float* result = (float*)malloc(sizeof(float) * (n + 1));
    for (int i = 0; i < n; i++) {
        float coeff = pts[i][1];
        for (int j = 0; j < i; j++) {
            float product = 1.0f;
            for (int k = 0; k < j; k++) {
                product *= (pts[i][0] - pts[k][0]);
            }
            coeff -= product * result[j];
        }
        result[i] = coeff;
    }
    return result;
}

float evalpoly(float* poly, int n, float x) {
    float result = poly[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        result = result * x + poly[i];
    }
    return result;
}


int main() {
    float points[10][2] = {
        {3.2, 7.5},
        {1.1, 4.8},
        {6.3, 2.2},
        {0.9, 5.4},
        {8.6, 3.3},
        {4.4, 6.6},
        {7.1, 0.5},
        {2.8, 9.0},
        {5.5, 1.7},
        {9.9, 8.8}
    };
    int numPoints = sizeof(points) / sizeof(points[0]);

    float **pts = (float**)malloc(sizeof(float*) * numPoints);

    for (int i = 0; i < numPoints; i++) {
        pts[i] = (float*)malloc(sizeof(float) * 2);
        pts[i][0] = points[i][0];
        pts[i][1] = points[i][1];
    }

    float ans_lin = linear_interpolation(1, points[0][0], points[0][1], points[1][0], points[1][1]);
    printf("Answer : %f\n", ans_lin);

    int n = 3;
    float* res = get_norder(n, pts, numPoints);
    float ans_norder = evalpoly(res,n,1);
    printf("Answer : %f\n", ans_norder);


    return 0;
}
