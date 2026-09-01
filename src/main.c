#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "cell.h"
#include "board.h"
#include "args.h"

void print_board(int size, Cell board[][size], bool is_reveal_all);

int main(int argc, char **argv) {
    MinesweeperArgs args = parse_minesweeper_args(argc, argv);
    int size = args.size;
    int numMines = args.numMines;
    bool isMaterialized = false;

    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");

    Cell board[size][size];
    init_board(size, board);

    int row, col;
    char cmd;

    print_board(size, board, false);

    while (true) {
        printf("Enter cmd (f/r/q), row, column (0-%d): ", size - 1);
        scanf(" %c %d %d", &cmd, &row, &col);

        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("Invalid row or column\n");
            continue;
        }

        if (cmd == 'r') {
            if (!isMaterialized) {
                place_mine(size, numMines, row, col, board);
                compute_counts(size, board);

                isMaterialized = true;
            }

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

void print_board(int size, Cell board[][size], bool is_reveal_all) {
    printf("    ");
    for (int col = 0; col < size; col++) {
        printf("%d ", col); // Broken if digit is larger than 9
    }
    printf("\n    ");
    for (int col = 0; col < size; col++) {
        printf("--");
    }
    printf("\n");
    for (int row = 0; row < size; row++) {
        printf("%d | ", row);
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
        printf(" | %d\n", row);
    }
    printf("    ");
    for (int col = 0; col < size; col++) {
        printf("--");
    }
    printf("\n    ");
    for (int col = 0; col < size; col++) {
        printf("%d ", col);
    }
    printf("\n");
}
