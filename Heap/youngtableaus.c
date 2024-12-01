#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printMat(int** mat,int rows,int cols){
    for(int i = 0;i<rows;i++){
        for(int j = 0;j<cols;j++){
            printf("%d\t",mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printPat(int n){
    for(int i = 0 ;i<n;i++){
        printf("*");
    }
    printf("\n");
}


void balance(int** youngt, int rows, int cols, int x, int y) {
    if (x >= rows || y >= cols) {
        return;
    }

    int down = (x + 1 < rows) ? youngt[x + 1][y] : INT_MAX;
    int right = (y + 1 < cols) ? youngt[x][y + 1] : INT_MAX;

    if (down < right && down < youngt[x][y]) {
        // Swap with down
        int temp = youngt[x + 1][y];
        youngt[x + 1][y] = youngt[x][y];
        youngt[x][y] = temp;
        balance(youngt, rows, cols, x + 1, y); 
    }
    else if (right < youngt[x][y]) {
        // Swap with right
        int temp = youngt[x][y + 1];
        youngt[x][y + 1] = youngt[x][y];
        youngt[x][y] = temp;
        balance(youngt, rows, cols, x, y + 1);
    }
}


int extract_min(int** youngt, int rows, int cols) {
    int ans = youngt[0][0];
    youngt[0][0] = INT_MAX;
    balance(youngt, rows, cols, 0, 0); 
    return ans;
}

int check(int** youngt, int rows, int cols, int val) {
    int start = 0;
    int end = rows - 1;
    int mid;

    while (start <= end) {
        mid = (start + end) / 2;

        if (youngt[mid][0] < val) {
            start = mid + 1;
        } else if (youngt[mid][0] > val) {
            end = mid - 1;
        } else {
            break;
        }
    }

    if (start > end) {
        return -1; 
    }

    int row = mid;
    start = 0;
    end = cols - 1;

    while (start <= end) {
        int col_mid = (start + end) / 2;

        if (youngt[row][col_mid] < val) {
            start = col_mid + 1;
        } else if (youngt[row][col_mid] > val) {
            end = col_mid - 1;
        } else {
            return 1;
        }
    }

    return -1; 
}



void insert(int** young_t, int rows, int cols, int val) {
    int i = rows - 1;
    int j = cols - 1;
    int x, y, max, temp;

    if (young_t[i][j] != INT_MAX) {
        printf("Overflow\n");
        return;
    }

    young_t[i][j] = val;
    max = INT_MAX;

    while ((i > 0 || j > 0)) {
        x = i;
        y = j;
        
        if (i - 1 >= 0 && young_t[i][j] < young_t[i - 1][j]) {
            x = i - 1;
            y = j;
        }
        if (j - 1 >= 0 && young_t[x][y] < young_t[i][j - 1]) {
            x = i;
            y = j - 1;
        }

        if (x != i || y != j) {
            temp = young_t[x][y];
            young_t[x][y] = young_t[i][j];
            young_t[i][j] = temp;

            i = x;
            j = y;
        } else {
            break;
        }
    }
}

int main(){
    srand(100);
    int rows = 10;
    int cols = 15;
    int** youngt = (int**)(malloc(sizeof(int*) * rows));

    for(int i = 0;i<rows;i++){
        youngt[i] = (int*)malloc(sizeof(int)*cols);
        for(int j = 0;j<cols;j++){
            youngt[i][j] = INT_MAX;
        }
    }

    for(int i = 0;i<150;i++){
        insert(youngt,rows,cols,(rand()%100));
    }

    printMat(youngt,rows,cols);
    printPat(100);

    int min;
    for(int i = 0;i<30;i++) min = extract_min(youngt,rows,cols);

    printMat(youngt,rows,cols);
    return 0;
}