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

int main(void) {
    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");

    char board[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    place_mine(board);
    print_board(board);

    return 0;
}
