#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int buffer[100] = {0};

// #include <cstdlib>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int roles(int role,int prev_user_decision){
    int decision;
    if(role == 1){
        // giver
        decision = 1;
    }
    else if(role == 2){
        // cheater
        decision = 0;
    }
    else if(role == 3){
        // copy cat
        if(prev_user_decision == -1){
            decision = 1;
        }
        else{

            decision = prev_user_decision;
        }
    }
    else if(role == 4){
        // grudger
        if(prev_user_decision == 0){
            buffer[0] = 0;
        }
        else if(prev_user_decision == -1){
            buffer[0] = 1;
        }
        if(buffer[0] == 0 ){
            decision = 0;
        }
        else{
            decision = 1;
        }
    }
    else if(role == 5){

        if(prev_user_decision == 0){
            buffer[5] == 1;
        }

        if(buffer[5] == 1){
            decision = prev_user_decision;
        }
        else{
            decision = 0;
        }

        if(buffer [0] == 4 || prev_user_decision == -1){
            buffer[0] = 4;
            decision = 1;
            buffer[0]--;
        }
        else if(buffer [0] == 3){
            decision = 0;
            buffer[0]--;
        }
        else if(buffer [0] == 2){
            decision = 1;
            buffer[0]--;
        }
        else if(buffer [0] == 1){
            decision = 1;
            buffer[0]--;
        }

    }
    else if(role == 6){
        if(prev_user_decision == -1){
            decision = rand() & 10000;
        }
        else{
            if(prev_user_decision == 1){
                buffer[1]++;
            }
            else{
                buffer[0]++;
            }

            if(buffer[1]>buffer[0])decision = 1;
            else if(buffer[1]<buffer[0]) decision = 0;
            else decision = rand() & 10000;
        }
    }
    return decision;
}

void calculate_points(int comp_decision,int user_decision,int* comp_points,int* user_points){
    if(comp_decision == 0){
        *comp_points += 0;
        *user_points += 0;
    }
    if(comp_decision == 1){
        *comp_points += -1;
        *user_points += 3;
    }
    if(user_decision == 0){
        *user_points += 0;
        *comp_points += 0;
    }
    if(user_decision == 1){
        *comp_points += 3;
        *user_points += -1;
    }
}

void printPatt1(int n){
    printf("\n\n");
    for(int i = 0;i<n;i++){
        printf("*");
    }
    printf("\n\n");
}

int main() {
    char* rules = 
        "Zoro's Adventure: Welcome to the Human World!\n\n"
        "Hello there, fellow traveler! I am Zoro, the bravest dog you'll ever meet, and I have a thrilling challenge for you. "
        "Let me guide you into the strange and unpredictable world of humans. In this world, people come in all flavors  "
        "some kind-hearted, others cunning, and many somewhere in between. You, my friend, must learn to survive and outsmart them.\n\n"
        "Let me tell you about the fascinating characters you'll encounter:\n"
        "1) The Givers: These humans are the epitome of generosity. They would sacrifice everything  yes, even their own lives  for the greater good. Noble, aren't they?\n"
        "2) The Cheaters: Beware! These selfish beings care only about themselves. They'll do anything to gain an advantage, even if it means betraying you.\n"
        "3) The Copycats: These ones are like mirrors  treat them well, and they'll reflect your kindness. Cross them, and they'll mirror your treachery.\n"
        "4) The Grudgers: Emotional and intense, they start with pure cooperation. But be warned  betray their trust even once, and they'll never forgive you. Ever.\n"
        "5) The Detectives: These are the sharp minds, the strategists. They’ll study your every move, analyze your actions, and make decisions based on your past behavior. Outsmarting them won't be easy!\n"
        "6) The Majority Movers: These humans are followers of trends. They’ll align their moves with whatever action you ve taken the most. Predictable yet tricky.\n\n"
        "Your mission is simple but dangerous: survive and thrive in this simulation by playing against one of these fascinating types of humans.\n\n"
        "The Rules of the Game of Trust:\n"
        "In front of you is a mysterious machine. If you put a coin into the machine, your opponent gains three coins  and vice versa.\n"
        "Each of you has a single choice:\n"
        "- COOPERATE (put in a coin)\n"
        "- CHEAT (don’t put in a coin)\n"
        "Choose wisely, for your fate hangs on the balance of trust and strategy.\n";

    printf("-------------------------|||  Welcome to the Game of Trust  |||---------------------\n\n");
    printf("%s", rules);

    printPatt1(100);
    printf("Enter the number of rounds you dare to play: ");
    int n_rounds;
    scanf("%d", &n_rounds);

    printf("\nWho will you face? Choose your opponent:\n");
    printf("1) The Givers\n2) The Cheaters\n3) The Copycats\n4) The Grudgers\n5) The Detectives\n6) The Majority Movers\n");
    int role;
    scanf("%d", &role);

    printPatt1(100);

    int user_decision = -1;
    int comp_decision = -1;
    int user_cheat = 0;
    int comp_cheat = 0;
    int user_points = 0;
    int comp_points = 0;

    for (int i = 1; i <= n_rounds; i++) {
        printPatt1(100);
        printf("\nPrepare for the next round!\n");
        printf("Round %d begins!\n", i);

        comp_decision = roles(role, user_decision);

        printf("\nEnter your decision (0 to CHEAT, 1 to COOPERATE): ");
        scanf("%d", &user_decision);
        if (i < n_rounds) {
            clearScreen();
        }

        if (user_decision == 0) user_cheat++;
        if (comp_decision == 0) {
            comp_cheat++;
            printf("\nThe HUMAN CHEATED this round!\n");
        } else {
            printf("\nThe HUMAN COOPERATED this round!\n");
        }

        calculate_points(comp_decision, user_decision, &comp_points, &user_points);

        printf("\nTallying the points...\n");
        printf("Your total points: %d\n", user_points);
        printf("Opponent's total points: %d\n", comp_points);

    }

    printPatt1(100);

    if (user_points > comp_points) {
        printf("\n*** Victory! ***\n");
        printf("Congratulations, brave one! You have mastered the art of survival in the treacherous human world.\n");
    } else {
        printf("\n*** Defeat! ***\n");
        printf("The humans were too cunning for you this time. They have triumphed in the Game of Trust.\n");
    }

    return 0;
}