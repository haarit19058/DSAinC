// Escape the Maze - Full Game with CPU (A*)
// Console version for Linux (e.g., Arch Linux)

#include <iostream>
#include <vector>
#include <stack>
#include <queue>    // For std::priority_queue
#include <map>      // For std::map
#include <cstdlib>  // For rand, srand, system, abs (integer)
#include <ctime>    // For time
#include <cmath>    // For std::abs (though cstdlib abs is fine for int)
#include <algorithm>// For std::reverse
#include <utility>  // For std::pair
#include <tuple>    // For std::tie
#include <functional> // For std::greater

// For getch() on Linux
#include <termios.h>
#include <unistd.h>
#include <cstdio>   // For getchar

// Non-blocking character input for Linux
char getch() {
    termios oldt{}, newt{};
    char c;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

// Maze dimensions
constexpr int WIDTH = 10;
constexpr int HEIGHT = 10;

// Represents a single cell in the maze
struct Cell {
    bool visited = false;    // For maze generation
    bool wallTop = true;
    bool wallBottom = true;
    bool wallLeft = true;
    bool wallRight = true;
    bool hasKey = false;
    bool hasTrap = false;
};

// Represents a position (x, y) in the grid
struct Position {
    int x, y;

    // Needed for comparing positions (e.g., checking if goal is reached)
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    // Needed for using Position as a key in std::map
    bool operator<(const Position& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

// Game state variables
std::vector<std::vector<Cell>> grid(HEIGHT, std::vector<Cell>(WIDTH));
Position playerPos = {0, 0};
Position cpuPos = {WIDTH - 1, 0}; // CPU starts at top-right
Position goal = {WIDTH - 1, HEIGHT - 1}; // Exit is at bottom-right
Position keyPos; // Position of the key, to be set during maze generation
bool playerHasKey = false; // Changed variable name for clarity
int playerHealth = 100;

// Checks if a given coordinate (x, y) is within the maze boundaries
bool isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
}

// Gets unvisited neighbors of a cell for maze generation
std::vector<Position> getUnvisitedNeighbors(int x, int y) {
    std::vector<Position> neighbors;
    // Order: Up, Right, Down, Left
    if (isValid(x, y - 1) && !grid[y - 1][x].visited) neighbors.push_back({x, y - 1});
    if (isValid(x + 1, y) && !grid[y][x + 1].visited) neighbors.push_back({x + 1, y});
    if (isValid(x, y + 1) && !grid[y + 1][x].visited) neighbors.push_back({x, y + 1});
    if (isValid(x - 1, y) && !grid[y][x - 1].visited) neighbors.push_back({x - 1, y});
    return neighbors;
}

// Removes walls between two adjacent cells
void removeWalls(Position current, Position next) {
    int dx = next.x - current.x;
    int dy = next.y - current.y;

    if (dx == 1) { // Move right
        grid[current.y][current.x].wallRight = false;
        grid[next.y][next.x].wallLeft = false;
    } else if (dx == -1) { // Move left
        grid[current.y][current.x].wallLeft = false;
        grid[next.y][next.x].wallRight = false;
    } else if (dy == 1) { // Move down
        grid[current.y][current.x].wallBottom = false;
        grid[next.y][next.x].wallTop = false;
    } else if (dy == -1) { // Move up
        grid[current.y][current.x].wallTop = false;
        grid[next.y][next.x].wallBottom = false;
    }
}

// Generates the maze using Randomized Depth-First Search
void generateMaze() {
    // Reset grid cells (in case this function is called multiple times)
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = Cell(); // Reinitialize to default state
        }
    }
    
    std::stack<Position> s;
    s.push({0, 0}); // Start DFS from top-left cell
    grid[0][0].visited = true;

    while (!s.empty()) {
        Position current = s.top();
        std::vector<Position> neighbors = getUnvisitedNeighbors(current.x, current.y);

        if (!neighbors.empty()) {
            Position next = neighbors[rand() % neighbors.size()]; // Pick a random unvisited neighbor
            removeWalls(current, next);
            grid[next.y][next.x].visited = true;
            s.push(next);
        } else {
            s.pop(); // Backtrack
        }
    }

    // Place the key randomly, ensuring it's not at player start or goal
    do {
        keyPos = {rand() % WIDTH, rand() % HEIGHT};
    } while ((keyPos == playerPos || keyPos == cpuPos || keyPos == goal));
    grid[keyPos.y][keyPos.x].hasKey = true;

    // Place some traps randomly (approx 10% of cells)
    // Ensure traps are not at player start, cpu start, goal, or key position
    int numTraps = WIDTH * HEIGHT / 10;
    for (int i = 0; i < numTraps; ++i) {
        int tx, ty;
        do {
            tx = rand() % WIDTH;
            ty = rand() % HEIGHT;
        } while ((tx == playerPos.x && ty == playerPos.y) ||
                 (tx == cpuPos.x && ty == cpuPos.y) ||
                 (tx == goal.x && ty == goal.y) ||
                 (tx == keyPos.x && ty == keyPos.y));
        grid[ty][tx].hasTrap = true;
    }
}

// Prints the maze to the console
void printMaze() {
    system("clear"); // Use "clear" for Linux/macOS, "cls" for Windows
    for (int y = 0; y < HEIGHT; y++) {
        // Print top walls of cells in the current row
        for (int x = 0; x < WIDTH; x++) {
            std::cout << (grid[y][x].wallTop ? "+---" : "+   ");
        }
        std::cout << "+\n";

        // Print cell contents and left walls
        for (int x = 0; x < WIDTH; x++) {
            std::cout << (grid[y][x].wallLeft ? "|" : " ");
            if (playerPos.x == x && playerPos.y == y)
                std::cout << " P ";
            else if (cpuPos.x == x && cpuPos.y == y)
                std::cout << " C ";
            else if (goal.x == x && goal.y == y)
                std::cout << " E "; // E for Exit
            else if (grid[y][x].hasKey)
                std::cout << " K "; // K for Key
            else if (grid[y][x].hasTrap)
                std::cout << " X "; // X for Trap
            else
                std::cout << "   "; // Empty cell
        }
        std::cout << "|\n"; // Rightmost wall of the maze
    }

    // Print bottom walls of the last row
    for (int x = 0; x < WIDTH; x++) {
        std::cout << "+---";
    }
    std::cout << "+\n";

    // Print game status
    std::cout << "Health: " << playerHealth << " | Key: " << (playerHasKey ? "Yes" : "No") << "\n";
}

// Checks if movement from 'from' to 'to' is possible (no wall, within bounds)
bool canMove(Position from, Position to) {
    if (!isValid(to.x, to.y)) return false;

    int dx = to.x - from.x;
    int dy = to.y - from.y;

    if (dy == -1 && dx == 0) return !grid[from.y][from.x].wallTop;    // Moving Up
    if (dy == 1 && dx == 0) return !grid[from.y][from.x].wallBottom; // Moving Down
    if (dx == -1 && dy == 0) return !grid[from.y][from.x].wallLeft;   // Moving Left
    if (dx == 1 && dy == 0) return !grid[from.y][from.x].wallRight;  // Moving Right
    
    return false; // Should not happen for adjacent cells
}

// Processes player movement based on input character
void movePlayer(char input) {
    Position newPos = playerPos;
    if (input == 'w' || input == 'W') newPos.y--;
    else if (input == 's' || input == 'S') newPos.y++;
    else if (input == 'a' || input == 'A') newPos.x--;
    else if (input == 'd' || input == 'D') newPos.x++;
    else return; // Invalid input

    if (canMove(playerPos, newPos)) {
        playerPos = newPos;
        // Check for key pickup
        if (grid[playerPos.y][playerPos.x].hasKey) {
            playerHasKey = true;
            grid[playerPos.y][playerPos.x].hasKey = false; // Key is picked up
            std::cout << "You found the key!\n"; // Optional: feedback
        }
        // Check for traps
        if (grid[playerPos.y][playerPos.x].hasTrap) {
            playerHealth -= 20;
            grid[playerPos.y][playerPos.x].hasTrap = false; // Trap is triggered and removed
            std::cout << "Ouch! You hit a trap.\n"; // Optional: feedback
        }
    }
}

// Manhattan distance heuristic for A*
int manhattanDistance(Position a, Position b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// A* pathfinding algorithm
std::vector<Position> aStar(Position start, Position targetGoal) {
    // openSet stores {fScore, position}
    std::priority_queue<std::pair<int, Position>, std::vector<std::pair<int, Position>>, std::greater<std::pair<int, Position>>> openSet;
    
    std::map<Position, Position> cameFrom; // Stores the previous node in the optimal path
    std::map<Position, int> gScore;       // Cost from start to current node

    // Initialize gScore for all nodes to infinity
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            gScore[{x, y}] = 1e9; // A large value representing infinity
        }
    }
    
    gScore[start] = 0; // Cost from start to start is 0
    // fScore = gScore + heuristic. For start node, fScore = heuristic.
    openSet.push({manhattanDistance(start, targetGoal), start});

    while (!openSet.empty()) {
        Position current = openSet.top().second;
        openSet.pop();

        if (current == targetGoal) {
            // Reconstruct path
            std::vector<Position> path;
            Position temp = current;
            while (cameFrom.count(temp)) {
                path.push_back(temp);
                temp = cameFrom[temp];
            }
            // path.push_back(start); // Optional: include start if needed by caller
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Define potential neighbors (Up, Down, Left, Right)
        Position neighborOffsets[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        for (Position offset : neighborOffsets) {
            Position neighbor = {current.x + offset.x, current.y + offset.y};

            if (!isValid(neighbor.x, neighbor.y) || !canMove(current, neighbor)) {
                continue; // Skip invalid or blocked neighbors
            }

            int tentative_gScore = gScore[current] + 1; // Cost to move to neighbor is 1

            if (tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                int fScore = tentative_gScore + manhattanDistance(neighbor, targetGoal);
                openSet.push({fScore, neighbor});
            }
        }
    }
    return {}; // Return empty path if goal is not reachable
}

int main() {
    srand(time(0)); // Seed random number generator
    generateMaze();

    while (true) {
        printMaze();

        // Check win/loss conditions
        if (playerPos == goal && playerHasKey) {
            std::cout << "You reached the exit with the key. You win!\n";
            break;
        }
        if (playerPos == goal && !playerHasKey) {
            std::cout << "You reached the exit, but you don't have the key!\n";
            // Game continues, or you can make this a specific non-win state
        }
        if (cpuPos == goal) { // Assuming CPU doesn't need a key
            std::cout << "CPU reached the exit. You lose!\n";
            break;
        }
        if (playerHealth <= 0) {
            std::cout << "You ran out of health. Game over.\n";
            break;
        }

        // Player's turn
        std::cout << "Move (W/A/S/D): ";
        char input = getch(); // Get single character input without Enter
        std::cout << input << "\n"; // Echo input for user to see
        if (input == 'q' || input == 'Q') { // Quit option
            std::cout << "Quitting game.\n";
            break;
        }
        movePlayer(input);

        // CPU's turn (if player hasn't won/lost yet)
        if (!(playerPos == goal && playerHasKey) && playerHealth > 0) {
            Position cpuTarget = goal; // CPU always tries to reach the main goal
            // If player has the key, CPU might try to intercept player or reach goal faster
            // For simplicity, CPU just heads for the exit.
            
            std::vector<Position> cpuPath = aStar(cpuPos, cpuTarget);
            if (!cpuPath.empty()) {
                cpuPos = cpuPath[0]; // Move CPU one step along the path
            }
        }
    }
    return 0;
}