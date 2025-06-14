#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <vector>
using namespace std;
int** initializeBoard(int rows, int cols, mt19937 &rng) {
    uniform_int_distribution<int> dist(0, 1);
    int** board = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        board[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            board[i][j] = dist(rng);
        }
    }
    return board;
}
int** generatePattern(int rows, int cols, int patternType) {
    int** pattern = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        pattern[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            pattern[i][j] = (i + j + patternType) % 2;
        }
    }
    return pattern;
}
void printBoard(int** board, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}
void flipPair(int** b, int i1, int j1, int i2, int j2) {
    b[i1][j1] ^= 1;
    b[i2][j2] ^= 1;
}
int minMovesToTransform(int** board, int** target, int rows, int cols, vector<pair<int,int>> &movesList) {
    int** temp = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        temp[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            temp[i][j] = board[i][j];
        }
    }

    int moves = 0;
    movesList.clear();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (temp[i][j] != target[i][j]) {
                if (j + 1 < cols) {
                    temp[i][j] ^= 1;
                    temp[i][j+1] ^= 1;
                    movesList.emplace_back(i, j);
                    ++moves;
                } else if (i + 1 < rows) {
                    temp[i][j] ^= 1;
                    temp[i+1][j] ^= 1;
                    movesList.emplace_back(i, j);
                    ++moves;
                } else {
                    for (int k = 0; k < rows; ++k) {
                        delete[] temp[k];
                    }
                    delete[] temp;
                    return INT_MAX;
                }
            }
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (temp[i][j] != target[i][j]) {
                for (int k = 0; k < rows; ++k) {
                    delete[] temp[k];
                }
                delete[] temp;
                return INT_MAX;
            }
        }
    }
    for (int k = 0; k < rows; ++k) {
        delete[] temp[k];
    }
    delete[] temp;
    return moves;
}
void applyMoves(int** b, int rows, int cols, const vector<pair<int,int>> &moves) {
    for (auto &move : moves) {
        int i = move.first;
        int j = move.second;
        if (j + 1 < cols) {
            b[i][j] ^= 1;
            b[i][j+1] ^= 1;
        } else if (i + 1 < rows) {
            b[i][j] ^= 1;
            b[i+1][j] ^= 1;
        }
    }
}
void freeBoard(int** board, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] board[i];
    }
    delete[] board;
}
int main() {
    int rows, cols;
    cout << "Введите размеры доски (строки и столбцы): ";
    cin >> rows >> cols;
    random_device rd;
    mt19937 rng(rd());
    int** board = initializeBoard(rows, cols, rng);
    cout << "\nНачальная доска:\n";
    printBoard(board, rows, cols);
    int** pattern0 = generatePattern(rows, cols, 0);
    int** pattern1 = generatePattern(rows, cols, 1);
    vector<pair<int,int>> movesList0, movesList1;
    int res0 = minMovesToTransform(board, pattern0, rows, cols, movesList0);
    int res1 = minMovesToTransform(board, pattern1, rows, cols, movesList1);
    int minMoves = min(res0, res1);
    vector<pair<int,int>> *bestMoves = nullptr;
    int** bestPattern = nullptr;
    if (minMoves == res0) {
        bestMoves = &movesList0;
        bestPattern = pattern0;
    } else {
        bestMoves = &movesList1;
        bestPattern = pattern1;
    }
    if (minMoves == INT_MAX) {
        cout << "Невозможно достигнуть шахматного рисунка за допустимое число ходов.\n";
        freeBoard(board, rows);
        freeBoard(pattern0, rows);
        freeBoard(pattern1, rows);
        return 0;
    }
    int** finalBoard = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        finalBoard[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            finalBoard[i][j] = board[i][j];
        }
    }
    applyMoves(finalBoard, rows, cols, *bestMoves);
    cout << "\nМинимальное число ходов: " << minMoves << "\n";
    cout << "\nДоска после выполнения минимальных ходов:\n";
    printBoard(finalBoard, rows, cols);
    freeBoard(board, rows);
    freeBoard(pattern0, rows);
    freeBoard(pattern1, rows);
    freeBoard(finalBoard, rows);
    return 0;
}
