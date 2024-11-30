#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void nextConfig(int** mat, int row, int col) {
    int** temp = (int**)malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        temp[i] = (int*)malloc(col * sizeof(int));
        for (int j = 0; j < col; j++) {
            temp[i][j] = mat[i][j]; // Copy the current state
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (i > 0 && j > 0 && j + 1 < col && i + 1 < row) {
                if (mat[i][j-1] == 1 && mat[i-1][j] == 1) {
                    temp[i][j] = 1;
                    // temp[i+1][j] = 1;
                }
                if (mat[i][j-1] == 0 && mat[i-1][j] == 0) {
                    temp[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mat[i][j] = temp[i][j]; // Apply changes to the original matrix
        }
        free(temp[i]); // Free each row of the temporary matrix
    }
    free(temp); // Free the temporary matrix
}


void printMat(int** mat, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

int** generateRandom(int row, int col) {
    int** mat = (int**)malloc(sizeof(int*) * row);
    for (int i = 0; i < row; i++) {
        mat[i] = (int*)malloc(sizeof(int) * col);
        for (int j = 0; j < col; j++) {
        	if(i!=0 && i!= row-1 && j!=0 && j!= col-1){
            	mat[i][j] = rand() % 5?0:1;        		
        	}
        }
    }
    return mat;
}

void freeMat(int** mat, int row) {
    for (int i = 0; i < row; i++) {
        free(mat[i]);
    }
    free(mat);
}

void printPat(int n) {
    for (int i = 0; i < n; i++) {
        printf("*");
    }
    printf("\n");
}

int** lolmat() {
    int row = 16;
    int col = 25;
    int temp[16][25] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int** mat = (int**)malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        mat[i] = (int*)malloc(col * sizeof(int));
        for (int j = 0; j < col; j++) {
            mat[i][j] = temp[i][j];
        }
    }
    return mat;
}

int main() {
    srand(time(NULL));

    int row, col;
    printf("Enter the number of Rows: ");
    scanf("%d", &row);
    printf("Enter the number of Cols: ");
    scanf("%d", &col);

    int** mat = generateRandom(row, col);

    // int** mat = lolmat();
    // int row = sizeof(mat)/sizeof(mat[0]);
    // int col = sizeof(mat[0])/sizeof(mat[0][0]);


    system("clear");
    int c = 1;
    int counter = 0;
    while (c != 3) {
        printMat(mat, row, col);
    	
        // printPat(100);
        printf("\n\nNumber of gens: %d\n\n",counter);
        printf("Enter 1 for next gen or 3 to exit: ");
        scanf("%d", &c);
        if (c == 1) {
            nextConfig(mat, row, col);
            counter++;
        }
        system("clear");
    }

    freeMat(mat, row);
    return 0;
}
