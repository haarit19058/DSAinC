#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **playGround;

int num() {
    return (rand() % 10 == 0) ? 4 : 2; // 10% chance of 4, 90% chance of 2
}
void init(){
    int coord = rand()%16;
    int x = coord/4;
    int y = coord%4;

    playGround[x][y] = num();
}



void addNewNum() {
    int arr[16] = {0};
    int counter = 0;

    // Collect all empty positions in the grid
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (playGround[i][j] == 0) {
                arr[counter] = i * 4 + j; // Store position as a single integer
                counter++;
            }
        }
    }

    // If no empty positions are available, return
    if (counter == 0) return;

    // Choose a random empty position
    int random = arr[rand() % counter];
    int x = random / 4; // Row index
    int y = random % 4; // Column index

    // Place a new number (2 or 4) at the chosen empty position
    playGround[x][y] = num();
}


void moveUp() {
    for (int col = 0; col < 4; col++) {
        // Step 1: Shift non-zero values upwards
        for (int row = 0; row < 4; row++) {
            if (playGround[row][col] == 0) {
                for (int k = row + 1; k < 4; k++) {
                    if (playGround[k][col] != 0) {
                        playGround[row][col] = playGround[k][col];
                        playGround[k][col] = 0;
                        break;
                    }
                }
            }
        }
        // Step 2: Merge adjacent equal tiles
        for (int row = 0; row < 3; row++) {
            if (playGround[row][col] != 0 && playGround[row][col] == playGround[row + 1][col]) {
                playGround[row][col] *= 2;
                playGround[row + 1][col] = 0;
            }
        }
        // Step 3: Shift again to remove any gaps from merging
        for (int row = 0; row < 4; row++) {
            if (playGround[row][col] == 0) {
                for (int k = row + 1; k < 4; k++) {
                    if (playGround[k][col] != 0) {
                        playGround[row][col] = playGround[k][col];
                        playGround[k][col] = 0;
                        break;
                    }
                }
            }
        }
    }
}


void moveDown() {
    for (int col = 0; col < 4; col++) {
        // Step 1: Shift non-zero values downwards
        for (int row = 3; row >= 0; row--) {
            if (playGround[row][col] == 0) {
                for (int k = row - 1; k >= 0; k--) {
                    if (playGround[k][col] != 0) {
                        playGround[row][col] = playGround[k][col];
                        playGround[k][col] = 0;
                        break;
                    }
                }
            }
        }
        // Step 2: Merge adjacent equal tiles
        for (int row = 3; row > 0; row--) {
            if (playGround[row][col] != 0 && playGround[row][col] == playGround[row - 1][col]) {
                playGround[row][col] *= 2;
                playGround[row - 1][col] = 0;
            }
        }
        // Step 3: Shift again to remove any gaps from merging
        for (int row = 3; row >= 0; row--) {
            if (playGround[row][col] == 0) {
                for (int k = row - 1; k >= 0; k--) {
                    if (playGround[k][col] != 0) {
                        playGround[row][col] = playGround[k][col];
                        playGround[k][col] = 0;
                        break;
                    }
                }
            }
        }
    }
}


void moveLeft(){
    for (int row = 0; row < 4; row++) {
        // Step 1: Shift non-zero values upwards
        for (int col = 0; col < 4; col++) {
            if (playGround[row][col] == 0) {
                for (int k = col + 1; k < 4; k++) {
                    if (playGround[row][k] != 0) {
                        playGround[row][col] = playGround[row][k];
                        playGround[row][k] = 0;
                        break;
                    }
                }
            }
        }
        // Step 2: Merge adjacent equal tiles
        for (int col = 0; col < 3; col++) {
            if (playGround[row][col] != 0 && playGround[row][col] == playGround[row][col+1]) {
                playGround[row][col] *= 2;
                playGround[row][col+1] = 0;
            }
        }
        // Step 3: Shift again to remove any gaps from merging
        for (int col = 0; col < 4; col++) {
            if (playGround[row][col] == 0) {
                for (int k = col + 1; k < 4; k++) {
                    if (playGround[row][k] != 0) {
                        playGround[row][col] = playGround[row][k];
                        playGround[row][k] = 0;
                        break;
                    }
                }
            }
        }
    }
    
}

void moveRight(){
    for (int row = 0; row < 4; row++) {
        // Step 1: Shift non-zero values upwards
        for (int col = 3; col >=0; col--) {
            if (playGround[row][col] == 0) {
                for (int k = col -1; k >=0; k--) {
                    if (playGround[row][k] != 0) {
                        playGround[row][col] = playGround[row][k];
                        playGround[row][k] = 0;
                        break;
                    }
                }
            }
        }
        // Step 2: Merge adjacent equal tiles
        for (int col = 3; col >0; col--) {
            if (playGround[row][col] != 0 && playGround[row][col] == playGround[row][col-1]) {
                playGround[row][col] *= 2;
                playGround[row][col-1] = 0;
            }
        }
        // Step 3: Shift again to remove any gaps from merging
        for (int col = 3; col >= 0; col--) {
            if (playGround[row][col] == 0) {
                for (int k = col -1; k > 0; k--) {
                    if (playGround[row][k] != 0) {
                        playGround[row][col] = playGround[row][k];
                        playGround[row][k] = 0;
                        break;
                    }
                }
            }
        }
    }
}

int maxElem(){
    int max = -1;
    for(int i = 0;i<4;i++){
        for(int j = 0;j<4;j++){
            if(playGround[i][j]>max){
                max = playGround[i][j];
            }
        }
    }
    return max;
}



int isValid(){
    for(int i = 0;i<4;i++){
        for(int j = 0;j<4;j++){
            if(playGround[i][j] == 0)return 1;
        }
    }
    return 0;
}

// #include <stdio.h>

// #include <stdio.h>

void printMat(int** mat, int n, int m) {
    printf("\n\033[0;36m┌");
    for (int j = 0; j < m; j++) {
        printf("─────");
        if (j < m - 1) printf("┬");
    }
    printf("┐\033[0m\n");

    for (int i = 0; i < n; i++) {
        printf("\033[0;36m│\033[0m");
        for (int j = 0; j < m; j++) {
            int val = mat[i][j];
            if (val == 0) {
                printf("     ");
            } else {
                switch (val) {
                    case 2:    printf("\033[0;32m"); break;
                    case 4:    printf("\033[0;34m"); break;
                    case 8:    printf("\033[0;33m"); break;
                    case 16:   printf("\033[0;35m"); break;
                    case 32:   printf("\033[0;36m"); break;
                    case 64:   printf("\033[0;31m"); break;
                    case 128:  printf("\033[1;32m"); break;
                    case 256:  printf("\033[1;34m"); break;
                    case 512:  printf("\033[1;33m"); break;
                    case 1024: printf("\033[1;35m"); break;
                    case 2048: printf("\033[1;36m"); break;
                    default:   printf("\033[0;37m"); break;
                }
                printf(" %3d ", val);
                printf("\033[0m");
            }
            printf("\033[0;36m│\033[0m");
        }
        printf("\n");
        if (i < n - 1) {
            printf("\033[0;36m├");
            for (int j = 0; j < m; j++) {
                printf("─────");
                if (j < m - 1) printf("┼");
            }
            printf("┤\033[0m\n");
        }
    }

    printf("\033[0;36m└");
    for (int j = 0; j < m; j++) {
        printf("─────");
        if (j < m - 1) printf("┴");
    }
    printf("┘\033[0m\n\n");
}


int main(){
    srand(time(NULL));

    playGround = (int**)malloc(sizeof(int*) * 4);
    for(int i = 0;i<4;i++){
        *(playGround+i) = (int*)malloc(sizeof(int)*4);
        for(int j = 0;j<4;j++){
            playGround[i][j] = 0;
        }
    }

    char msg[100]  = "Welcome to the game 2048 .... \nPress any 1 to proceed...";
    int temp;
    printf("%s",msg);
    scanf("%d",&temp);

    system("clear");

    init();
    printMat(playGround,4,4);
    
    while (maxElem()<4096 && isValid())
    {
        printf("1 MoveUp -- 2 MoveDown -- 3 MoveLeft -- 4 MoveRight\n");
        int command;
        scanf("%d",&command);

        if(command == 1){
            moveUp();
        }
        if(command == 2){
            moveDown();
        }
        if(command == 3){
            moveLeft();
        }
        if(command == 4){
            moveRight();
        }

        addNewNum();
        system("clear");
        printMat(playGround,4,4);
    }
    

    return 0;
}