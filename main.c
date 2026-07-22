#include <stdio.h>

#define BOARD_SIZE 9

void init_board(char board[][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = '.';
        }
    }
}

void print_board(char board[][BOARD_SIZE]) {
    // TODO: use two nested for loops to print every cell.
    // Print a space between cells on the same row, and a newline
    // after each row finishes (hint: the newline goes after the
    // inner loop, not inside it).

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf("%c ", board[row][col]);
        }
        printf("\n");
    }
}

int main(void) {
    printf("Welcome to Minesweeper!\n");

    char board[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    print_board(board);

    return 0;
}
