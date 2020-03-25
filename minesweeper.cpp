/* Khai báo thư viện ------------------------------------------------------ */



#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>


using namespace std;



/* Kiểu dữ liệu mới ------------------------------------------------------- */


enum CellState { HIDDEN, SHOWN };       // Trạng thái của một ô (ẨN / HIỆN)



/* Hằng ------------------------------------------------------------------- */



const int WIDTH         = 4;            // Số lượng ô theo chiều ngang
const int HEIGHT        = 4;            // Số lượng ô theo chiều dọc
const int MINES_NUM     = 5;            // Số lượng mìn
const int CELL_WIDTH    = 2;            // Chiều rộng một ô khi vẽ (số kí tự)
const int MINE          = -1;           // Giá trị thể hiện một ô đang có mìn


/* Biến toàn cục ---------------------------------------------------------- */



// Giá trị hiện tại của các ô (là mìn hay có bao nhiêu mìn xung quanh)
int boardValues[HEIGHT][WIDTH];

// Trạng thái hiện tại của các ô (ẨN / HIỆN)
CellState boardStates[HEIGHT][WIDTH];



/* Nguyên mẫu hàm --------------------------------------------------------- */



// Sinh ngẫu nhiên MINES_NUM số tự nhiên khác nhau
// trong khoảng [0, WIDTH * HEIGHT)
// lưu vào mảng minePos
void randomMines(int minePos[]);

// Đếm và trả về số lượng mìn xung quanh vị trí (row, col)
int countMines(int row, int col);

// Khởi tạo các ô với trạng thái ban đầu là ẨN,
// giá trị các ô được tính từ hàm randomMines và countMines
void initBoard();

// Vẽ bảng lên màn hình.
// Nếu tham số showAll là true thì hiện cả những ô trạng thái đang là ẨN
void drawBoard(bool showAll = false);

// Yêu cầu người chơi nhập vào hàng và cột đến khi nào hợp lệ,
// sau đó lưu vào hai biến row và col
void pickCell(int &row, int &col);

// Kiểm tra ô (row, col) có phải là mìn hay không
bool isMine(int row, int col);

// Cập nhật trạng thái của ô (row, col) thành HIỆN.
// Liên tục HIỆN các ô xung quanh nếu ô đó có giá trị là 0,
// tức xung quanh không có mìn
void updateBoard(int row, int col);

// Kiểm tra xem người chơi đã thắng chưa (số ô chưa mở bằng số mìn)
bool isWin();

// Vẽ một dòng sử dụng các ký tự ' ' và '-'
void drawLine();



/* Định nghĩa hàm --------------------------------------------------------- */



int main() {
    srand(time(0)); // Khởi tạo seed cho hàm rand
    initBoard();

    while (true) {
        drawBoard();

        int row, col;
        pickCell(row, col);

        if (isMine(row, col)) {
            drawBoard(true);
            cout << endl << "YOU'RE DEAD!" << endl;
            return 0;
        }

        updateBoard(row, col);

        if (isWin()) {
            drawBoard(true);
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

    bool chosen[max]; // Mảng đánh dấu các số đã chọn để không random vào lại
    for (int i = 0; i < max; i ++)
        chosen[i] = false;

    int cnt = 0;
    while (cnt < MINES_NUM) {
        int num = rand() % max;
        if (!chosen[num]) {
            chosen[num] = true; // Đánh dẫu là đã chọn số num
            minePos[cnt] = num; // Lưu num vào mảng minePos
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
                    && boardValues[y][x] == MINE) {
                cnt ++;
            }
        }
    }
    return cnt;
}

void drawLine() {
    // Vẽ kí tự trắng với độ rộng là CELL_WIDTH + 1
    cout << setw(CELL_WIDTH + 1) << ' ';

    // Vẽ tổng cộng WIDTH ô, mỗi ô có CELL_WIDTH + 1 kí tự gạch ngang
    for (int i = 0; i < (CELL_WIDTH + 1) * WIDTH; i ++) {
        cout << '-';
    }
    cout << endl;
}

void drawBoard(bool showAll) {
    cout << endl << endl << "TOTAL MINES: " << MINES_NUM << endl << endl;

    cout << setw(CELL_WIDTH + 1) << ' ';
    for (int i = 0; i < WIDTH; i ++) {
        cout << setw(CELL_WIDTH) << (i + 1); // Vẽ số thứ tự của cột
        cout << '/';
    }
    cout << endl;

    drawLine();
    for (int i = 0; i < HEIGHT; i ++) {
        cout << setw(CELL_WIDTH) << (i + 1); // Vẽ số thứ tự của hàng
        cout << '/';
        for (int j = 0; j < WIDTH; j ++) {
            if (!showAll && boardStates[i][j] == HIDDEN) {
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

        // Nếu người chơi không nhập số, cin.fail() trả về true
        // Các kí tự người dùng nhập vẫn nằm trong bộ nhớ đệm của cin
        if (cin.fail()) {

            // Xoá bỏ trạng thái fail của cin
            cin.clear();

            // Xoá bỏ tối đa INT_MAX các kí tự trong bộ nhớ đệm của cin
            // đến khi gặp kí tự '\n' thì dừng.
            // INT_MAX là giá trị lớn nhất của kiểu dữ liệu int
            cin.ignore(INT_MAX, '\n');
            continue;
        }

        // Người chơi nhập số thứ tự hàng và cột tính từ 1
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

    // Chỉ loang nếu không có mìn xung quanh
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

/* Hết -------------------------------------------------------------------- */
