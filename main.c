#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARD_SIZE 9
#define NUM_MINES 10

typedef struct {
    bool is_mine;
    int adjacent_count;
    bool is_revealed;
    bool is_flagged;
} Cell;

void init_board(Cell board[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col].is_mine = false;
            board[row][col].adjacent_count = 0;
            board[row][col].is_revealed = false;
            board[row][col].is_flagged = false;
        }
    }
}

void place_mine(Cell board[][BOARD_SIZE]) {
    for (int mines = 0; mines < NUM_MINES; mines++) {

        int row;
        int col;

        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (board[row][col].is_mine);

        board[row][col].is_mine = true;
    }
}

void compute_counts(Cell board[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col].adjacent_count = 0;
        }
    }

    // Count the number of mines in each row
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
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
            if (row - 1 >= 0 && col + 1 < BOARD_SIZE) {
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
            if (col + 1 < BOARD_SIZE) {
                if (board[row][col + 1].is_mine) {
                    count++;
                }
            }
            // 1, -1
            if (row + 1 < BOARD_SIZE && col - 1 >= 0) {
                if (board[row + 1][col - 1].is_mine) {
                    count++;
                }
            }
            // 1, 0
            if (row + 1 < BOARD_SIZE) {
                if (board[row + 1][col].is_mine) {
                    count++;
                }
            }
            // 1, 1
            if (row + 1 < BOARD_SIZE && col + 1 < BOARD_SIZE) {
                if (board[row + 1][col + 1].is_mine) {
                    count++;
                }
            }

            board[row][col].adjacent_count = count;
        }
    }
}

void print_board(Cell board[][BOARD_SIZE], bool is_reveal_all) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (is_reveal_all) {
                if (board[row][col].is_mine) {
                    printf("* ");
                } else {
                    printf("%d ", board[row][col].adjacent_count);
                }
            } else {
                if (board[row][col].is_revealed) {
                    printf("%d ", board[row][col].adjacent_count);
                } else if (board[row][col].is_flagged) {
                    printf("F ");
                } else {
                    printf(". ");
                }
            }
        }
        printf("\n");
    }
}

void flood_fill(Cell board[][BOARD_SIZE], int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }

    if (board[row][col].is_revealed || board[row][col].is_flagged) {
        return;
    }

    board[row][col].is_revealed = true;

    if (board[row][col].adjacent_count != 0) {
        return;
    }

    flood_fill(board, row - 1, col - 1);
    flood_fill(board, row - 1, col);
    flood_fill(board, row - 1, col + 1);
    flood_fill(board, row, col - 1);
    flood_fill(board, row, col + 1);
    flood_fill(board, row + 1, col - 1);
    flood_fill(board, row + 1, col);
    flood_fill(board, row + 1, col + 1);
}

bool is_win(Cell board[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
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

int main(void) {
    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");

    Cell board[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    place_mine(board);
    compute_counts(board);

    int row, col;
    char cmd;

    while (true) {
        printf("Enter cmd (f/r/q), row, column (0-8): ");
        scanf(" %c %d %d", &cmd, &row, &col);

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            printf("Invalid row or column\n");
            continue;
        }

        if (cmd == 'r') {
            if (board[row][col].is_revealed) {
                printf("You have already revealed this cell\n");
                continue;
            }

            if (board[row][col].is_flagged) {
                printf("This cell is flagged. Unflag it first.\n");
                continue;
            }

            flood_fill(board, row, col);

            if (board[row][col].is_mine) {
                printf("You have lost!\n");
                print_board(board, true);
                break;
            }

            print_board(board, false);

            if (is_win(board)) {
                printf("You have won!\n");
                break;
            }
        } else if (cmd == 'f') {
            board[row][col].is_flagged = !board[row][col].is_flagged;

            print_board(board, false);
        } else if (cmd == 'q') {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
