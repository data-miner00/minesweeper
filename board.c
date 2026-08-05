#include <stdlib.h>
#include "board.h"

void init_board(int size, Cell board[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[row][col].is_mine = false;
            board[row][col].adjacent_count = 0;
            board[row][col].is_revealed = false;
            board[row][col].is_flagged = false;
        }
    }
}

void place_mine(int size, int numMines, Cell board[][size]) {
    for (int mines = 0; mines < numMines; mines++) {

        int row;
        int col;

        do {
            row = rand() % size;
            col = rand() % size;
        } while (board[row][col].is_mine);

        board[row][col].is_mine = true;
    }
}

void compute_counts(int size, Cell board[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[row][col].adjacent_count = 0;
        }
    }

    // Count the number of mines in each row
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col].is_mine) {
                board[row][col].adjacent_count = -1;
                continue;
            }

            int count = 0;
            // -1, -1
            if (row - 1 >= 0 && col - 1 >= 0) {
                if (board[row - 1][col - 1].is_mine) {
                    count++;
                }
            }
            // -1, 0
            if (row - 1 >= 0) {
                if (board[row - 1][col].is_mine) {
                    count++;
                }
            }
            // -1, 1
            if (row - 1 >= 0 && col + 1 < size) {
                if (board[row - 1][col + 1].is_mine) {
                    count++;
                }
            }
            // 0, -1
            if (col - 1 >= 0) {
                if (board[row][col - 1].is_mine) {
                    count++;
                }
            }
            // 0, 1
            if (col + 1 < size) {
                if (board[row][col + 1].is_mine) {
                    count++;
                }
            }
            // 1, -1
            if (row + 1 < size && col - 1 >= 0) {
                if (board[row + 1][col - 1].is_mine) {
                    count++;
                }
            }
            // 1, 0
            if (row + 1 < size) {
                if (board[row + 1][col].is_mine) {
                    count++;
                }
            }
            // 1, 1
            if (row + 1 < size && col + 1 < size) {
                if (board[row + 1][col + 1].is_mine) {
                    count++;
                }
            }

            board[row][col].adjacent_count = count;
        }
    }
}

void flood_fill(int size, Cell board[][size], int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        return;
    }

    if (board[row][col].is_revealed || board[row][col].is_flagged) {
        return;
    }

    board[row][col].is_revealed = true;

    if (board[row][col].adjacent_count != 0) {
        return;
    }

    flood_fill(size, board, row - 1, col - 1);
    flood_fill(size, board, row - 1, col);
    flood_fill(size, board, row - 1, col + 1);
    flood_fill(size, board, row, col - 1);
    flood_fill(size, board, row, col + 1);
    flood_fill(size, board, row + 1, col - 1);
    flood_fill(size, board, row + 1, col);
    flood_fill(size, board, row + 1, col + 1);
}

// When all cells that are not mines are revealed, the game is won
bool is_win(int size, Cell board[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col].is_mine) {
                continue;
            }

            if (!board[row][col].is_revealed) {
                return false;
            }
        }
    }

    return true;
}

bool is_lose(int size, Cell board[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col].is_mine && board[row][col].is_revealed) {
                return true;
            }
        }
    }

    return false;
}
