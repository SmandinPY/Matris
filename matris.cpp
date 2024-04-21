#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 24;
const char PLAYER_CHAR = '>';
const char PATH_CHAR = ' ';
const char FALLING_CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int FALLING_SPEED = 100; // Milliseconds
const int PATH_WIDTH = 10;
const int SCORE_INCREMENT = 10;
const int SCORE_PER_SECOND = 500;

enum ConsoleColor {
    GREEN,
    PURPLE,
    YELLOW,
    RED,
    BLUE,
    WHITE
};

class Player {
public:
    int x, y;

    Player(int initialX, int initialY) : x(initialX), y(initialY) {}
};

class Game {
private:
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
    Player player;
    int score;
    int timeAlive;
    bool isRunning;

public:
    Game() : player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 2), score(0), timeAlive(0), isRunning(true) {}

    void initialize() {
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                screen[i][j] = ' ';
            }
        }

        for (int i = 0; i < PATH_WIDTH; ++i) {
            screen[SCREEN_HEIGHT - 1][SCREEN_WIDTH / 2 - PATH_WIDTH / 2 + i] = PATH_CHAR;
        }
    }

    void update() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a' && player.x > 0)
                player.x--;
            else if (key == 'd' && player.x < SCREEN_WIDTH - 1)
                player.x++;
        }

        for (int i = SCREEN_HEIGHT - 2; i >= 0; --i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                if (screen[i][j] != ' ') {
                    char temp = screen[i][j];
                    screen[i][j] = ' ';
                    if (i < SCREEN_HEIGHT - 1)
                        screen[i + 1][j] = temp;
                }
            }
        }

        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            if (rand() % 10 == 0) {
                char fallingChar = FALLING_CHARS[rand() % (sizeof(FALLING_CHARS) - 1)];
                ConsoleColor color = static_cast<ConsoleColor>(rand() % 6);
                setColor(color);
                screen[0][j] = fallingChar;
                setColor(WHITE);
            }
        }

        if (screen[player.y][player.x] != PATH_CHAR) {
            isRunning = false;
        }

        score += SCORE_INCREMENT;

        timeAlive++;
        score += SCORE_PER_SECOND / 10;
    }

    void draw() {
        system("cls");
        
        screen[player.y][player.x] = PLAYER_CHAR;
         cout << "Score: " << score << endl;

        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                cout << screen[i][j];
            }
            cout << endl;
        }

        if (!isRunning) {
            int gameoverX = SCREEN_WIDTH / 2 - 4;
            int gameoverY = SCREEN_HEIGHT / 2;
            cout << "\n\n\n\n\n";
            for (int i = 0; i < gameoverY; ++i) {
                cout << endl;
            }
            for (int i = 0; i < gameoverX; ++i) {
                cout << " ";
            }
            cout << "GAME OVER!" << endl;
            for (int i = 0; i < gameoverY; ++i) {
                cout << endl;
            }
            Sleep(2000);
        }
    }

    bool isGameOver() {
        return !isRunning;
    }

    void run() {
        while (!isGameOver()) {
            update();
            draw();
            Sleep(FALLING_SPEED);
        }
        cout << "Final Score: " << score << endl;
    }

    void setColor(ConsoleColor color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, static_cast<int>(color));
    }
};

int main() {
    srand(time(nullptr));

    Game game;
    game.initialize();
    game.run();

    return 0;
}
