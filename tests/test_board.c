#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "cell.h"
#define TEST_SIZE 3

static void test_init_board(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    init_board(TEST_SIZE, board);

    for (int row = 0; row < TEST_SIZE; row++) {
        for (int col = 0; col < TEST_SIZE; col++) {
            assert(!board[row][col].is_mine);
            assert(board[row][col].adjacent_count == 0);
            assert(!board[row][col].is_revealed);
            assert(!board[row][col].is_flagged);
        }
    }
}

static void test_place_mine(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    init_board(TEST_SIZE, board);

    place_mine(TEST_SIZE, 1, board); // Can I parameterize this?

    int total_mines = 0;
    for (int row = 0; row < TEST_SIZE; row++) {
        for (int col = 0; col < TEST_SIZE; col++) {
            if (board[row][col].is_mine) {
                total_mines++;
            }
        }
    }
    assert(total_mines == 1);
}

static void test_compute_counts(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    init_board(TEST_SIZE, board);

    // Place a mine in the middle
    board[1][1].is_mine = true;

    // Place a mine in the top left
    board[0][0].is_mine = true;

    int expected_adjacent_counts[TEST_SIZE][TEST_SIZE] = {
        {-1, 2, 1},
        {2, -1, 1},
        {1, 1, 1},
    };

    compute_counts(TEST_SIZE, board);

    for (int row = 0; row < TEST_SIZE; row++) {
        for (int col = 0; col < TEST_SIZE; col++) {
            assert(board[row][col].adjacent_count == expected_adjacent_counts[row][col]);
        }
    }
}

static void test_flood_fill(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    init_board(TEST_SIZE, board);

    // Place a mine in the top left
    board[0][0].is_mine = true;

    int expected_adjacent_counts[TEST_SIZE][TEST_SIZE] = {
        {-1, 1, 0},
        {1, 1, 0},
        {0, 0, 0},
    };

    // Compute the counts
    compute_counts(TEST_SIZE, board);

    // click on zero
    flood_fill(TEST_SIZE, board, 0, 2);

    // Check the counts
    for (int row = 0; row < TEST_SIZE; row++) {
        for (int col = 0; col < TEST_SIZE; col++) {
            assert(board[row][col].adjacent_count == expected_adjacent_counts[row][col]);
        }
    }

    // Check that the cells were revealed
    bool is_revealed[TEST_SIZE][TEST_SIZE] = {
        {false, true, true},
        {true, true, true},
        {true, true, true},
    };

    for (int row = 0; row < TEST_SIZE; row++) {
        for (int col = 0; col < TEST_SIZE; col++) {
            assert(board[row][col].is_revealed == is_revealed[row][col]);
        }
    }
}

static void test_is_win(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    init_board(TEST_SIZE, board);

    // Place a mine in the middle
    board[1][1].is_mine = true;

    // Open up all the cells besides the mine
    board[0][0].is_revealed = true;
    board[1][0].is_revealed = true;
    board[2][0].is_revealed = true;
    board[0][1].is_revealed = true;
    board[2][1].is_revealed = true;

    // Three non-mine cells (0,2), (1,2), (2,2) are still unrevealed
    assert(!is_win(TEST_SIZE, board));

    // Reveal the remaining non-mine cells; the mine itself stays unrevealed
    board[0][2].is_revealed = true;
    board[1][2].is_revealed = true;
    board[2][2].is_revealed = true;

    assert(is_win(TEST_SIZE, board));
}

static void test_is_lose(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    init_board(TEST_SIZE, board);

    // Place a mine in the middle
    board[1][1].is_mine = true;

    // Mine is placed but not yet revealed: not a loss
    assert(!is_lose(TEST_SIZE, board));

    // Click on the mine
    board[1][1].is_revealed = true;

    assert(is_lose(TEST_SIZE, board));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <test_name>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "init_board") == 0) {
        test_init_board();
    } else if (strcmp(argv[1], "place_mine") == 0) {
        test_place_mine();
    } else if (strcmp(argv[1], "compute_counts") == 0) {
        test_compute_counts();
    } else if (strcmp(argv[1], "flood_fill") == 0) {
        test_flood_fill();
    } else if (strcmp(argv[1], "is_win") == 0) {
        test_is_win();
    } else if (strcmp(argv[1], "is_lose") == 0) {
        test_is_lose();
    } else {
        fprintf(stderr, "unknown test: %s\n", argv[1]);
        return 1;
    }

    printf("PASS\n");
    return 0;
}
