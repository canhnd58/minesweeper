#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH         = 4;
const int HEIGHT        = 4;
const int MINES_NUM     = 5;
const int CELL_WIDTH    = 2;

const int MINE          = -1;

enum CellState { HIDDEN, SHOWN };

int boardValues[HEIGHT][WIDTH];
CellState boardStates[HEIGHT][WIDTH];

void initBoard();
void drawBoard(bool hidden = true);
void pickCell(int &, int &);
bool isMine(int, int);
void updateBoard(int, int);
bool isWin();
void randomMines(int []);
int countMines(int, int);

void drawLine(char symbol = '-');

int main() {
    srand(time(0));
    initBoard();

    while (true) {
        drawBoard();

        int row, col;
        pickCell(row, col);

        if (isMine(row, col)) {
            drawBoard(false);
            cout << endl << "YOU'RE DEAD!" << endl;
            return 0;
        }

        updateBoard(row, col);

        if (isWin()) {
            drawBoard(false);
            cout << endl << "YOU WIN!" << endl;
            return 0;
        }
    }

    return 0;
}

void initBoard() {
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            boardStates[i][j] = HIDDEN;
            boardValues[i][j] = 0;
        }
    }

    int minePos[MINES_NUM];
    randomMines(minePos);

    for (int i = 0; i < MINES_NUM; i ++) {
        int pos = minePos[i];
        boardValues[pos/WIDTH][pos%WIDTH] = MINE;
    }

    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            if (boardValues[i][j] != MINE) {
                boardValues[i][j] = countMines(i, j);
            }
        }
    }
}

void randomMines(int minePos[]) {
    int max = WIDTH * HEIGHT;
    bool chosen[max];
    for (int i = 0; i < max; i ++)
        chosen[i] = false;

    int cnt = 0;
    while (cnt < MINES_NUM) {
        int num = rand() % max;
        if (!chosen[num]) {
            minePos[cnt] = num;
            chosen[num] = true;
            cnt ++;
        }
    }
}

int countMines(int row, int col) {
    int cnt = 0;
    for (int i = -1; i <= 1; i ++) {
        for (int j = -1; j <= 1; j ++) {
            int y = row + i;
            int x = col + j;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT
                    && boardValues[y][x] == MINE)
                cnt ++;
        }
    }
    return cnt;
}

void drawLine(char symbol) {
    cout << setw(CELL_WIDTH + 1) << ' ';
    for (int i = 0; i < (1 + CELL_WIDTH) * WIDTH; i ++) {
        cout << symbol;
    }
    cout << endl;
}

void drawBoard(bool hidden) {
    cout << endl << endl << "TOTAL MINES: " << MINES_NUM << endl << endl;
    cout << setw(CELL_WIDTH + 1) << ' ';
    for (int i = 0; i < WIDTH; i ++) {
        cout << setw(CELL_WIDTH) << (i + 1);
        cout << '/';
    }
    cout << endl;

    drawLine('~');
    for (int i = 0; i < HEIGHT; i ++) {
        cout << setw(CELL_WIDTH) << (i + 1);
        cout << "/";
        for (int j = 0; j < WIDTH; j ++) {
            if (hidden && boardStates[i][j] == HIDDEN) {
                cout << setw(CELL_WIDTH) << ' ';
            } else if (boardValues[i][j] == MINE) {
                cout << setw(CELL_WIDTH) << 'x';
            } else {
                cout << setw(CELL_WIDTH) << boardValues[i][j];
            }
            cout << '|';
        }
        cout << endl;
        drawLine();
    }
}

void pickCell(int &row, int &col) {
    int x, y;
    while (true) {
        cout << "Choose row and col: ";
        cin >> y >> x;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        row = y - 1;
        col = x - 1;

        if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH
            && boardStates[row][col] != SHOWN)
            break;
    }
}

bool isMine(int row, int col) {
    return boardValues[row][col] == MINE;
}

void updateBoard(int row, int col) {
    boardStates[row][col] = SHOWN;
    if (boardValues[row][col] != 0)
        return;

    for (int i = -1; i <= 1; i ++) {
        for (int j = -1; j <= 1; j ++) {
            int y = row + i;
            int x = col + j;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT
                    && boardStates[y][x] == HIDDEN) {
                updateBoard(y, x);
            }
        }
    }
}

bool isWin() {
    int cnt = 0;

    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            if (boardStates[i][j] != SHOWN) {
                cnt ++;
            }
        }
    }

    return cnt == MINES_NUM;
}
