#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX_LENGTH 1000

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen() {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cs;
    GetConsoleScreenBufferInfo(hconsole, &cs);
    consoleWidth = cs.srWindow.Right - cs.srWindow.Left + 1;
    consoleHeight = cs.srWindow.Bottom - cs.srWindow.Top + 1;
}

class Point {
public:
    int x_axis;
    int y_axis;
    Point() : x_axis(0), y_axis(0) {}
    Point(int x, int y) { x_axis = x; y_axis = y; }
};

class Snake {
private:
    int length;
    char direction;
public:
    Point snakeBody[MAX_LENGTH];
    Snake(int x, int y) {
        length = 1;
        snakeBody[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength() { return length; }

    Point getHead() const { return snakeBody[0]; }

    void reset(int x, int y) {
        length = 1;
        snakeBody[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    void changeDirection(char userDirection) {
        if ((userDirection == DIR_UP && direction != DIR_DOWN) ||
            (userDirection == DIR_DOWN && direction != DIR_UP) ||
            (userDirection == DIR_LEFT && direction != DIR_RIGHT) ||
            (userDirection == DIR_RIGHT && direction != DIR_LEFT)) {
            direction = userDirection;
        }
    }

    // Returns false if collision (self or wall)
    bool move(const Point& food) {
        for (int i = length - 1; i > 0; i--) {
            snakeBody[i] = snakeBody[i - 1];
        }

        switch (direction) {
            case DIR_UP:    snakeBody[0].y_axis--; break;
            case DIR_DOWN:  snakeBody[0].y_axis++; break;
            case DIR_LEFT:  snakeBody[0].x_axis--; break;
            case DIR_RIGHT: snakeBody[0].x_axis++; break;
        }

        // Check wall collision (borders are at x==0,x==consoleWidth-1,y==1,y==consoleHeight-2)
        if (snakeBody[0].x_axis <= 0 || snakeBody[0].x_axis >= consoleWidth-1 ||
            snakeBody[0].y_axis <= 1 || snakeBody[0].y_axis >= consoleHeight-1) {
            return false;
        }

        // Check self collision
        for (int i = 1; i < length; i++) {
            if (snakeBody[i].x_axis == snakeBody[0].x_axis && snakeBody[i].y_axis == snakeBody[0].y_axis) {
                return false;
            }
        }

        // Eat food
        if (food.x_axis == snakeBody[0].x_axis && food.y_axis == snakeBody[0].y_axis) {
            if (length + 1 < MAX_LENGTH) {
                snakeBody[length] = Point(snakeBody[length - 1].x_axis, snakeBody[length - 1].y_axis);
                length++;
            }
        }

        return true;
    }
};

class Board {
private:
    Snake* snake;
    const char snake_body_shape = 'O';
    Point food;
    const char food_shape = 'o';
    int score;
    int highScore;

    void loadHighScore() {
        highScore = 0;
        ifstream in("highscore.txt");
        if (in.is_open()) in >> highScore;
    }

    void saveHighScore() {
        ofstream out("highscore.txt");
        if (out.is_open()) out << highScore;
    }

public:
    Board() {
        initScreen();
        int sx = max(2, consoleWidth/2);
        int sy = max(3, consoleHeight/2);
        snake = new Snake(sx, sy);
        score = 0;
        loadHighScore();
        spawnFood();
    }

    ~Board() {
        saveHighScore();
        delete snake;
    }

    int getScore() { return score; }
    int getHighScore() { return highScore; }

    void gotoxy(int x, int y) {
        COORD coord; coord.X = x; coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void drawBorder() {
        // top border (row 1) and bottom border (consoleHeight-1)
        for (int x = 0; x < consoleWidth; x++) {
            gotoxy(x, 1); cout << '#';
            gotoxy(x, consoleHeight-1); cout << '#';
        }
        for (int y = 1; y < consoleHeight; y++) {
            gotoxy(0, y); cout << '#';
            gotoxy(consoleWidth-1, y); cout << '#';
        }
    }

    void displayCurrentScore() {
        gotoxy(0,0);
        cout << "Score:" << score << "  High:" << highScore << "    (WASD/Arrows) P=Pause";
    }

    bool isOnSnake(int x, int y) {
        for (int i = 0; i < snake->getLength(); i++) {
            if (snake->snakeBody[i].x_axis == x && snake->snakeBody[i].y_axis == y) return true;
        }
        return false;
    }

    void spawnFood() {
        int x, y;
        do {
            x = 1 + rand() % (max(1, consoleWidth-2));
            y = 2 + rand() % (max(1, consoleHeight-3));
        } while (isOnSnake(x,y));
        food = Point(x,y);
    }

    void drawFoodAndSnake() {
        system("cls");
        displayCurrentScore();
        drawBorder();

        // draw food
        gotoxy(food.x_axis, food.y_axis);
        cout << food_shape;

        // draw snake
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->snakeBody[i].x_axis, snake->snakeBody[i].y_axis);
            cout << snake_body_shape;
        }
    }

    // returns false when game over
    bool update() {
        bool alive = snake->move(food);
        if (!alive) return false;
        if (food.x_axis == snake->snakeBody[0].x_axis && food.y_axis == snake->snakeBody[0].y_axis) {
            score++;
            if (score > highScore) highScore = score;
            spawnFood();
        }
        return true;
    }

    void getKeyInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224 || key == 0) { // arrow keys
                key = _getch();
                switch (key) {
                    case 72: snake->changeDirection(DIR_UP); break;
                    case 80: snake->changeDirection(DIR_DOWN); break;
                    case 75: snake->changeDirection(DIR_LEFT); break;
                    case 77: snake->changeDirection(DIR_RIGHT); break;
                }
            } else {
                switch (toupper(key)) {
                    case 'W': snake->changeDirection(DIR_UP); break;
                    case 'S': snake->changeDirection(DIR_DOWN); break;
                    case 'A': snake->changeDirection(DIR_LEFT); break;
                    case 'D': snake->changeDirection(DIR_RIGHT); break;
                    case 'P': {
                        // pause until any key
                        gotoxy(consoleWidth/2 - 5, consoleHeight/2);
                        cout << "Paused - press any key";
                        _getch();
                        break;
                    }
                    default: break;
                }
            }
        }
    }

    void reset() {
        int sx = max(2, consoleWidth/2);
        int sy = max(3, consoleHeight/2);
        snake->reset(sx, sy);
        score = 0;
        spawnFood();
    }
};

int main(void) {
    srand((unsigned)time(0));
    initScreen();

    Board* board = new Board();

    bool running = true;
    while (running) {
        // Play a single round
        while (board->update()) {
            board->getKeyInput();
            board->drawFoodAndSnake();
            int delay = max(40, 200 - board->getScore() * 10);
            Sleep(delay);
        }

        // Game over
        board->drawFoodAndSnake();
        board->gotoxy(consoleWidth/2 - 5, consoleHeight/2 - 1);
        cout << "=== GAME OVER ===";
        board->gotoxy(consoleWidth/2 - 8, consoleHeight/2);
        cout << "Final score: " << board->getScore();
        board->gotoxy(consoleWidth/2 - 12, consoleHeight/2 + 1);
        cout << "Press R to restart, Q to quit";

        // wait for R or Q
        bool chosen = false;
        while (!chosen) {
            int k = _getch();
            if (k == 'r' || k == 'R') {
                board->reset();
                chosen = true;
            } else if (k == 'q' || k == 'Q') {
                chosen = true;
                running = false;
            }
        }
    }

    delete board;
    return 0;
}
