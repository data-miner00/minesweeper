#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 9
#define NUM_MINES 10


void init_board(char board[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = '.';
        }
    }
}

void print_board(char board[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf("%c ", board[row][col]);
        }
        printf("\n");
    }
}

void place_mine(char board[][BOARD_SIZE]) {
    for (int mines = 0; mines < NUM_MINES; mines++) {

        int row;
        int col;

        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (board[row][col] == '*');

        board[row][col] = '*';
    }
}

void compute_counts(char original[][BOARD_SIZE], char counts[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            counts[row][col] = '0';
        }
    }

    // Count the number of mines in each row
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (original[row][col] == '*') {
                counts[row][col] = '*';
                continue;
            }

            int count = 0;
            // -1, -1
            if (row - 1 >= 0 && col - 1 >= 0) {
                if (original[row - 1][col - 1] == '*') {
                    count++;
                }
            }
            // -1, 0
            if (row - 1 >= 0) {
                if (original[row - 1][col] == '*') {
                    count++;
                }
            }
            // -1, 1
            if (row - 1 >= 0 && col + 1 < BOARD_SIZE) {
                if (original[row - 1][col + 1] == '*') {
                    count++;
                }
            }
            // 0, -1
            if (col - 1 >= 0) {
                if (original[row][col - 1] == '*') {
                    count++;
                }
            }
            // 0, 1
            if (col + 1 < BOARD_SIZE) {
                if (original[row][col + 1] == '*') {
                    count++;
                }
            }
            // 1, -1
            if (row + 1 < BOARD_SIZE && col - 1 >= 0) {
                if (original[row + 1][col - 1] == '*') {
                    count++;
                }
            }
            // 1, 0
            if (row + 1 < BOARD_SIZE) {
                if (original[row + 1][col] == '*') {
                    count++;
                }
            }
            // 1, 1
            if (row + 1 < BOARD_SIZE && col + 1 < BOARD_SIZE) {
                if (original[row + 1][col + 1] == '*') {
                    count++;
                }
            }

            counts[row][col] = count + '0';
        }
    }
}

int main(void) {
    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");

    char board[BOARD_SIZE][BOARD_SIZE];
    char counts[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    place_mine(board);
    print_board(board);
    compute_counts(board, counts);
    print_board(counts);

    return 0;
}
