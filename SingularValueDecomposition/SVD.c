#include <stdio.h>
#include <stdlib.h>

// Allocate a matrix of floats with given dimensions.
float** getMat(int rows, int cols) {
    float** A = malloc(sizeof(float*) * rows);
    if (!A) {
        fprintf(stderr, "Memory allocation failed for matrix.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        A[i] = malloc(sizeof(float) * cols);
        if (!A[i]) {
            fprintf(stderr, "Memory allocation failed for matrix row %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return A;
}

// Free the allocated matrix.
void freeMat(float** A, int rows) {
    for (int i = 0; i < rows; i++) {
        free(A[i]);
    }
    free(A);
}

// Print the matrix.
void printMat(float** A, int rows, int cols) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Generate a random matrix (using integer values converted to float).
float** generateRandMat(int rows, int cols) {
    float** A = getMat(rows, cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            A[i][j] = (float)(rand() % 200);
        }
    }
    return A;
}

// Read a matrix from input.
float** takeMat(int rows, int cols) {
    float** A = getMat(rows, cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            // Use %f because the matrix is of type float.
            scanf("%f", &A[i][j]);
        }
    }
    return A;
}

// Multiply matrix A (of size rowsA x colsA) with matrix B (of size rowsB x colsB).
float** multiplyMat(float** A, int rowsA, int colsA, float** B, int rowsB, int colsB) {
    if (colsA != rowsB) {
        fprintf(stderr, "Matrix multiplication dimension mismatch.\n");
        return NULL;
    }
    float** mat = getMat(rowsA, colsB);
    for (int i = 0; i < rowsA; i++){
        for (int j = 0; j < colsB; j++){
            float sum = 0;
            for (int k = 0; k < colsA; k++){
                sum += A[i][k] * B[k][j];
            }
            mat[i][j] = sum;
        }
    }
    return mat;
}

// Compute the inverse of a square matrix using Gauss-Jordan elimination.
// Note: This function modifies the matrix A. If you wish to preserve A,
// pass a copy of A.
float** inverse(float** A, int n) {
    // Create an identity matrix of size n x n.
    float** I = getMat(n, n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            I[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    
    // Perform Gauss-Jordan elimination.
    for (int i = 0; i < n; i++){
        if (A[i][i] == 0) {
            fprintf(stderr, "Matrix is singular, cannot find inverse.\n");
            freeMat(I, n);
            return NULL;
        }
        float pivot = A[i][i];
        // Normalize the pivot row.
        for (int j = 0; j < n; j++){
            A[i][j] /= pivot;
            I[i][j] /= pivot;
        }
        // Eliminate the current column elements in other rows.
        for (int k = 0; k < n; k++){
            if (k != i) {
                float factor = A[k][i];
                for (int j = 0; j < n; j++){
                    A[k][j] -= factor * A[i][j];
                    I[k][j] -= factor * I[i][j];
                }
            }
        }
    }
    return I;
}





int main() {
    int rowsA, colsA;
    printf("Enter the number of rows and columns for the square matrix: ");
    scanf("%d %d", &rowsA, &colsA);
    if (rowsA != colsA) {
        fprintf(stderr, "Matrix must be square for inversion.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter matrix elements:\n");
    float** A = takeMat(rowsA, colsA);
    
    // Make a copy of A because the inversion function modifies the matrix.
    float** A_copy = getMat(rowsA, colsA);
    for (int i = 0; i < rowsA; i++){
        for (int j = 0; j < colsA; j++){
            A_copy[i][j] = A[i][j];
        }
    }
    
    float** invA = inverse(A_copy, rowsA);
    if (!invA) {
        fprintf(stderr, "Matrix inversion failed.\n");
        freeMat(A, rowsA);
        freeMat(A_copy, rowsA);
        return EXIT_FAILURE;
    }
    
    // Multiply A with its inverse. The product should be the identity matrix.
    float** B = multiplyMat(A, rowsA, colsA, invA, rowsA, colsA);
    if (!B) {
        fprintf(stderr, "Matrix multiplication failed.\n");
        freeMat(A, rowsA);
        freeMat(A_copy, rowsA);
        freeMat(invA, rowsA);
        return EXIT_FAILURE;
    }
    
    printf("Product of A and its inverse (should be the identity matrix):\n");
    printMat(B, rowsA, colsA);
    
    // Free allocated memory.
    freeMat(A, rowsA);
    freeMat(A_copy, rowsA);
    freeMat(invA, rowsA);
    freeMat(B, rowsA);
    
    return EXIT_SUCCESS;
}
