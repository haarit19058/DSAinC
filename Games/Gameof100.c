#include <stdio.h>
#include <stdlib.h>

// Simple strategy for computer's move
int strategy(int num) {
    if (num + 10 >= 100) {
        return 100; // Computer aims to win
    } else if(num+10>=90){
        return 89; // Computer tries to maximize its move
    }
    else{
        return num+10;
    }
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    int num = 0;
    int player = 1; // Start with player 1
    int temp;

    // Game description
    const char* que = "The game of 100 goes like this: I pick a number between 1 and 10, "
                      "then you pick one within the next ten numbers, and so on. "
                      "The first person to reach 100 wins.";
    printf("%s\n\n", que);

    // Choose mode
    printf("Enter 0 to play with the computer, 1 to play in multiplayer mode, or any other key to exit.\n");
    int flow;
    scanf("%d", &flow);

    if (flow == 0) { // Player vs Computer
        while (num < 100) {
            if (player == 0) { // Computer's turn
                num = strategy(num);
                printf("Computer chooses: %d\n", num);
            } else { // Player's turn
                printf("Player %d ----- Enter number between %d and %d: ", player, num + 1, num + 10);
                scanf("%d", &temp);
                
                if (temp >= num + 1 && temp <= num + 10) {
                    num = temp;
                } else {
                    printf("Invalid number! Please re-enter a number between %d and %d.\n", num + 1, num + 10);
                    continue;
                }
                clear_screen(); // Clear the screen after every turn
            }
            player = 1 - player; // Alternate turns
        }
    } else if (flow == 1) { // Multiplayer mode
        while (num < 100) {
            printf("Player %d ----- Enter number between %d and %d: ", player + 1, num + 1, num + 10);
            scanf("%d", &temp);

            if (temp >= num + 1 && temp <= num + 10) {
                num = temp;
            } else {
                printf("Invalid number! Please re-enter a number between %d and %d.\n", num + 1, num + 10);
                continue;
            }
            clear_screen(); // Clear the screen after every turn
            player = 1 - player; // Alternate turns
        }
    } else {
        printf("Exiting the game...\n");
        return 0;
    }

    printf("Player %d wins!\n", 1 - player); // The player who didn't move last wins
    return 0;
}
