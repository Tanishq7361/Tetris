#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;
const vector<int> COLORS = {1, 2, 3, 4, 5, 6, 7};
int highestScore = 0;
int difficulty = 5;

void gameLoop();

void displayIntro() {
    cout << "\033[1;36m===============================\033[0m" << endl;
    cout << "\033[1;35m    TEAM AURA++ PRESENTS\033[0m" << endl;
    cout << "\033[1;32m   THE NOSTALGIC FUN GAME \033[0m" << endl;
    cout << "\033[1;33m       TETRA TWIST \033[0m" << endl;
    cout << "\033[1;36m===============================\033[0m" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    system("clear");
}

void loadHighestScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highestScore;
        file.close();
    }
}

void saveHighestScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highestScore;
        file.close();
    }
}

void selectDifficulty() {
    int choice;
    
    while (true) {
        cout << "\033[1;36m═════════════════════════════════════\033[0m\n";
        cout << "\033[1;33m      Select Difficulty Level\033[0m\n";
        cout << "\033[1;36m═════════════════════════════════════\033[0m\n";
        cout << "\033[1;32m1. Easy  (Slow)\033[0m\n";
        cout << "\033[1;33m2. Medium\033[0m\n";
        cout << "\033[1;31m3. Hard  (Fast)\033[0m\n";
        cout << "\033[1;36m═════════════════════════════════════\033[0m\n";
        cout << "\033[1;34mEnter choice (1-3): \033[0m";

        cin >> choice;

        // Check if input is valid
        if (!cin.fail() && (choice >= 1 && choice <= 3)) {
            break; // Valid choice, exit loop
        }

        // Clear input buffer and display error message
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[1;31mInvalid choice! Please enter 1, 2, or 3.\033[0m\n\n";
    }

    switch (choice) {
        case 1: difficulty = 10; break;
        case 2: difficulty = 7; break;
        case 3: difficulty = 5; break;
    }
}

struct Tetromino {
    vector<vector<int>> shape;
    int x, y;
    int color;
};

vector<Tetromino> tetrominoes = {
    {{{1, 1, 1, 1}}, 3, 0, 0},
    {{{1, 1}, {1, 1}}, 3, 0, 0},
    {{{0, 1, 0}, {1, 1, 1}}, 3, 0, 0},
    {{{0, 1, 1}, {1, 1, 0}}, 3, 0, 0},
    {{{1, 1, 0}, {0, 1, 1}}, 3, 0, 0},
    {{{1, 0, 0}, {1, 1, 1}}, 3, 0, 0},
    {{{0, 0, 1}, {1, 1, 1}}, 3, 0, 0}
};

vector<vector<int>> board(HEIGHT, vector<int>(WIDTH, 0));
vector<vector<int>> colorBoard(HEIGHT, vector<int>(WIDTH, 0));
Tetromino currentTetromino;
int score = 0;
int fallCounter = 0;
bool isPaused = false;

void updateHighestScore() {
    if (score > highestScore) {
        highestScore = score;
        saveHighestScore();
    }
}

void drawBoard() {
    cout << "\033[H";
    vector<vector<int>> tempBoard = board;
    vector<vector<int>> tempColors = colorBoard;
    
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) {
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) {
            if (currentTetromino.shape[i][j]) {
                int x = currentTetromino.x + j;
                int y = currentTetromino.y + i;
                if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
                    tempBoard[y][x] = 1;
                    tempColors[y][x] = currentTetromino.color;
                }
            }
        }
    }
    
    cout << "   +";
    for (int j = 0; j < WIDTH; j++) cout << "---+";
    cout << "\n";
    
    for (int i = 0; i < HEIGHT; i++) {
        cout << "   |";
        for (int j = 0; j < WIDTH; j++) {
            if (tempBoard[i][j]) {
                cout << " \033[3" << tempColors[i][j] << "m■\033[0m |";
            } else {
                cout << "   |";
            }
        }
        cout << "\n   +";
        for (int j = 0; j < WIDTH; j++) cout << "---+";
        cout << "\n";
    }
    cout << " Score: " << score << "  |  Highest Score: " << highestScore << "\n";
    cout.flush();
}

bool isValidMove(int dx, int dy) {
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) {
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) {
            if (currentTetromino.shape[i][j]) {
                int newX = currentTetromino.x + j + dx;
                int newY = currentTetromino.y + i + dy;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX]))
                    return false;
            }
        }
    }
    return true;
}

void rotateTetromino() {
    int rows = currentTetromino.shape.size();
    int cols = currentTetromino.shape[0].size();
    vector<vector<int>> rotated(cols, vector<int>(rows, 0));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotated[j][rows - 1 - i] = currentTetromino.shape[i][j];
        }
    }
    
    Tetromino temp = currentTetromino;
    temp.shape = rotated;
    
     bool canRotate = true;
    for (size_t i = 0; i < temp.shape.size(); i++) {
        for (size_t j = 0; j < temp.shape[i].size(); j++) {
            if (temp.shape[i][j]) {
                int newX = temp.x + j;
                int newY = temp.y + i;

                // Check if out of bounds or collides with existing blocks
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX])) {
                    canRotate = false;
                    break;
                }
            }
        }
        if (!canRotate) break;
    }

    // Apply rotation only if valid
    if (canRotate) {
        currentTetromino = temp;
    }
    
}

void mergeTetromino() {
    for (size_t i = 0; i < currentTetromino.shape.size(); i++) {
        for (size_t j = 0; j < currentTetromino.shape[i].size(); j++) {
            if (currentTetromino.shape[i][j]) {
                board[currentTetromino.y + i][currentTetromino.x + j] = 1;
                colorBoard[currentTetromino.y + i][currentTetromino.x + j] = currentTetromino.color;
            }
        }
    }
}

void spawnTetromino() {
    currentTetromino = tetrominoes[rand() % tetrominoes.size()];
    currentTetromino.x = 3;
    currentTetromino.y = 0;
    currentTetromino.color = COLORS[rand() % COLORS.size()];
}

void removeFullLines() {
vector<int> rowsToRemove;  // Stores indices of rows to be deleted

    // Identify all full rows
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < WIDTH; j++) {
            if (!board[i][j]) {
                full = false;
                break;
            }
        }
        if (full) {
            rowsToRemove.push_back(i);
        }
    }

    // If multiple rows are full, remove them all together
    if (!rowsToRemove.empty()) {
        for (int row : rowsToRemove) {
            board.erase(board.begin() + row);
            colorBoard.erase(colorBoard.begin() + row);
        }

        // Add new empty rows at the top
        for (size_t i = 0; i < rowsToRemove.size(); i++) {
            board.insert(board.begin(), vector<int>(WIDTH, 0));
            colorBoard.insert(colorBoard.begin(), vector<int>(WIDTH, 0));
        }

        // Increase score based on the number of cleared rows
        score += rowsToRemove.size() * 100;
    }
}

char getKeyPress() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);  // Get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();  // Read a single key
    if (ch == '\033') {
        getchar();  // Skip '['
        switch (getchar()) {
            case 'A': ch = 'w'; break;  // Up arrow
            case 'B': ch = 's'; break;  // Down arrow
            case 'C': ch = 'd'; break;  // Right arrow
            case 'D': ch = 'a'; break;  // Left arrow
        }
    }
    return ch;
}

void gameOverScreen() {
    cout << "\n\033[1;31mGAME OVER!\033[0m\n";
    cout << "\033[1;32mFinal Score: \033[1;33m" << score << "\033[0m | \033[1;36mHighest Score: \033[1;33m" << highestScore << "\033[0m\n\n";
    cout << "\033[1;33mThank you for playing! \033[1;33m\n";
    cout << "Developed by: ";
    cout << "\033[1;34mYASH OZA\033[0m\n";
    cout << "              \033[1;34mDHVANIT SHAH\033[0m\n";
    cout << "              \033[1;34mTANISHQ SHAH\033[0m\n";
    cout << "              \033[1;34mYASH VAGHELA\033[0m\n";
    cout << "              \033[1;34mOM SUTARIYA\033[0m\n";
    this_thread::sleep_for(chrono::seconds(1));
    
    cout << "\033[?25h"; 
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Flush all previous key inputs
    char ch;
    while (read(STDIN_FILENO, &ch, 1) > 0);  // Read & discard any pending input

    cout << "\033[1;34mPress ENTER to play again or ESC to quit.\033[0m\n";

    while (true) {
        ch = getchar();
        if (ch == 27) { // ESC key
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            exit(0);
        } else if (ch == '\n') { // ENTER key
            
            board = vector<vector<int>>(HEIGHT, vector<int>(WIDTH, 0));
            colorBoard = vector<vector<int>>(HEIGHT, vector<int>(WIDTH, 0));
            score = 0;
            fallCounter = 0;
            isPaused = false;
            
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            system("clear");
            cout << "\033[?25l";
            
            gameLoop();
            return;
        }
    }
}

void gameLoop() {
    spawnTetromino();
    while (true) {
        if (!isPaused) {
            system("clear");
            drawBoard();
        }
        else {
            system("clear");
            drawBoard();
            cout << "\033[1;33mGame Paused. Press 'Q' to Resume.\033[0m\n";
            while (true) {
                char ch = getKeyPress();
                if (ch == 'q') {
                    isPaused = false;
                    system("clear");
                    break;  // Exit pause loop and resume game
                }
                else if (ch==27) {gameOverScreen(); return;}
            }
            continue;
        }
        if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == 0) {
            char ch = getKeyPress();
            switch (ch) {
                case 'a': case 68: if (isValidMove(-1, 0)) currentTetromino.x--; break;
                case 'd': case 67: if (isValidMove(1, 0)) currentTetromino.x++; break;
                case 's': case 66: if (isValidMove(0, 1)) currentTetromino.y++; break;
                case 'w': case 65: rotateTetromino(); break;
                case ' ': while (isValidMove(0, 1)) currentTetromino.y++; break;
                case 'q': isPaused = !isPaused; break;
                case 27: gameOverScreen(); return;
            }
        }
        
        if(!isPaused) {
        fallCounter++;
            int dynamicDifficulty = max(2, difficulty - (score / 100)); // Increase speed as score increases
            if (fallCounter >= dynamicDifficulty) {
                if (isValidMove(0, 1)) currentTetromino.y++;
                else {
                    mergeTetromino();
                    removeFullLines();
                    spawnTetromino();
                    if (!isValidMove(0, 0)) {gameOverScreen(); return;}
                }
                fallCounter = 0;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main() {

    system("clear");
    cout << "\033[?25l"; // Hide cursor
    displayIntro();
    srand(time(0));
    selectDifficulty();
    loadHighestScore();
    gameLoop();
    cout << "\033[?25h";
    return 0;
}
