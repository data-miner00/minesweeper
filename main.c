#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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

void init_revealed(bool revealed[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            revealed[row][col] = false;
        }
    }
}

void print_display(char board[][BOARD_SIZE], bool revealed[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (revealed[row][col] == true) {
                printf("%c ", board[row][col]);
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void flood_fill(char count[][BOARD_SIZE], bool revealed[][BOARD_SIZE], int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }

    if (revealed[row][col]) {
        return;
    }

    revealed[row][col] = true;

    if (count[row][col] != '0') {
        return;
    }

    flood_fill(count, revealed, row - 1, col - 1);
    flood_fill(count, revealed, row - 1, col);
    flood_fill(count, revealed, row - 1, col + 1);
    flood_fill(count, revealed, row, col - 1);
    flood_fill(count, revealed, row, col + 1);
    flood_fill(count, revealed, row + 1, col - 1);
    flood_fill(count, revealed, row + 1, col);
    flood_fill(count, revealed, row + 1, col + 1);
}

bool is_win(char counts[][BOARD_SIZE], bool revealed[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (counts[row][col] == '*') {
                continue;
            }

            if (revealed[row][col] == false) {
                return false;
            }
        }
    }

    return true;
}


int main(void) {
    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");

    char board[BOARD_SIZE][BOARD_SIZE];
    char counts[BOARD_SIZE][BOARD_SIZE];
    bool revealed[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    place_mine(board);
    compute_counts(board, counts);
    init_revealed(revealed);

    int row, col;

    while (true) {
        printf("Enter row and column (0-8): ");
        scanf("%d %d", &row, &col);

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            printf("Invalid row or column\n");
            continue;
        }

        flood_fill(counts, revealed, row, col);

        if (board[row][col] == '*') {
            printf("You have lost!\n");
            print_board(counts);
            break;
        }

        print_display(counts, revealed);

        if (is_win(counts, revealed)) {
            printf("You have won!\n");
            break;
        }
    }

    return 0;
}
