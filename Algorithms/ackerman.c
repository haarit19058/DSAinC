#include <stdio.h>
#include <stdlib.h>

void printMat(int** arr,int rows,int cols){
    for(int i = 0;i<rows;i++){
        for(int j = 0;j<cols;j++){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    int n,m;
    n = 20,m = 20;

    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        dp[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    // Fill the table iteratively
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i][j] = j + 1; // Base case: A(0, n) = n + 1
            } else if (j == 0 && i > 0) {
                dp[i][j] = dp[i - 1][1]; // A(m, 0) = A(m-1, 1)
            } else {
                dp[i][j] = dp[i - 1][dp[i][j - 1]]; // A(m, n) = A(m-1, A(m, n-1))
            }
        }
    }

    printMat(dp,n,m);

    return 0;
}