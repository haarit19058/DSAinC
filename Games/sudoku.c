#include <stdio.h>
#include <stdlib.h>

#define SIZE 9

// Function to print the Sudoku grid.
void printGrid(int **grid, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

// Check whether placing 'num' in grid[row][col] is valid.
int isValid(int **grid, int row, int col, int num, int size) {
    // Check the row and column.
    for (int i = 0; i < size; i++) {
        if (grid[row][i] == num || grid[i][col] == num)
            return 0;
    }

    // Check the 3x3 subgrid.
    int subRowStart = (row / 3) * 3;
    int subColStart = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[subRowStart + i][subColStart + j] == num)
                return 0;
        }
    }

    return 1;
}

// Recursive function to solve the Sudoku puzzle.
int solveSudoku(int **grid, int row, int col, int size) {
    // If reached the end of the grid, the puzzle is solved.
    if (row == size)
        return 1;

    // If reached end of row, move to next row.
    if (col == size)
        return solveSudoku(grid, row + 1, 0, size);

    // Skip the cell if it is already filled.
    if (grid[row][col] != 0)
        return solveSudoku(grid, row, col + 1, size);

    // Try all possible numbers.
    for (int num = 1; num <= size; num++) {
        if (isValid(grid, row, col, num, size)) {
            grid[row][col] = num;
            if (solveSudoku(grid, row, col + 1, size))
                return 1;
            // Backtrack if placing num doesn't lead to a solution.
            grid[row][col] = 0;
        }
    }

    // If no valid number is found, trigger backtracking.
    return 0;
}

int main() {
    int size = SIZE;

    // Allocate memory for the grid.
    int **grid = (int **)malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++) {
        grid[i] = (int *)malloc(sizeof(int) * size);
    }

    // Read the Sudoku grid from input.
    // Example input (0 denotes an empty cell):
    // 0 0 0 0 7 0 6 0 0
    // 6 3 0 4 5 9 1 0 0
    // 0 9 0 0 0 1 0 4 0
    // 0 0 0 3 0 0 0 0 0
    // 0 4 0 0 1 5 0 2 0
    // 0 1 5 0 2 0 0 8 0
    // 0 0 0 0 0 0 0 0 6
    // 0 0 1 0 0 6 0 0 9
    // 0 0 0 0 4 0 0 0 2
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &grid[i][j]);
        }
    }

    if (solveSudoku(grid, 0, 0, size)) {
        printf("\n\n\n***********************************Solved Sudoku****************************************\n\n\n");
        printGrid(grid, size);
    } else {
        printf("No solution exists.\n");
    }

    // Free the allocated memory.
    for (int i = 0; i < size; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}
