#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    bool is_mine;
    int adjacent_count;
    bool is_revealed;
    bool is_flagged;
} Cell;

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

void print_board(int size, Cell board[][size], bool is_reveal_all) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
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

int main(int argc, char** argv) {
    int size = 9;
    int numMines = 10;
    if (argc > 1) {
        if (strcmp(argv[1], "--size") == 0 || strcmp(argv[1], "-s") == 0) {
            size = atoi(argv[2]);
        } else if (strcmp(argv[1], "--mines") == 0 || strcmp(argv[1], "-m") == 0) {
            numMines = atoi(argv[2]);
        }
    }
    if (argc > 3) {
        if (strcmp(argv[3], "--size") == 0 || strcmp(argv[3], "-s") == 0) {
            size = atoi(argv[4]);
        } else if (strcmp(argv[3], "--mines") == 0 || strcmp(argv[3], "-m") == 0) {
            numMines = atoi(argv[4]);
        }
    }

    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");

    Cell board[size][size];
    init_board(size, board);
    place_mine(size, numMines, board);
    compute_counts(size, board);

    int row, col;
    char cmd;

    while (true) {
        printf("Enter cmd (f/r/q), row, column (0-%d): ", size - 1);
        scanf(" %c %d %d", &cmd, &row, &col);

        if (row < 0 || row >= size || col < 0 || col >= size) {
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

            flood_fill(size, board, row, col);

            if (board[row][col].is_mine) {
                printf("You have lost!\n");
                print_board(size, board, true);
                break;
            }

            print_board(size, board, false);

            if (is_win(size, board)) {
                printf("You have won!\n");
                break;
            }
        } else if (cmd == 'f') {
            board[row][col].is_flagged = !board[row][col].is_flagged;

            print_board(size, board, false);
        } else if (cmd == 'q') {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
