#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "cell.h"
#define TEST_SIZE 3

// Same setup as test_flood_fill() in test_board.c: one mine at (0,0).
static void setup_board(Cell board[TEST_SIZE][TEST_SIZE]) {
    init_board(TEST_SIZE, board);
    board[0][0].is_mine = true;
    compute_counts(TEST_SIZE, board);
}

static void assert_expected_result(Cell board[TEST_SIZE][TEST_SIZE]) {
    int expected_adjacent_counts[TEST_SIZE][TEST_SIZE] = {
        {-1, 1, 0},
        {1, 1, 0},
        {0, 0, 0},
    };
    bool expected_revealed[TEST_SIZE][TEST_SIZE] = {
        {false, true, true},
        {true, true, true},
        {true, true, true},
    };

    for (int row = 0; row < TEST_SIZE; row++) {
        for (int col = 0; col < TEST_SIZE; col++) {
            assert(board[row][col].adjacent_count == expected_adjacent_counts[row][col]);
            assert(board[row][col].is_revealed == expected_revealed[row][col]);
        }
    }
}

static void test_flood_fill_stack(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    setup_board(board);

    flood_fill_stack(TEST_SIZE, board, 0, 2);

    assert_expected_result(board);
}

static void test_flood_fill_queue(void) {
    Cell board[TEST_SIZE][TEST_SIZE];
    setup_board(board);

    flood_fill_queue(TEST_SIZE, board, 0, 2);

    assert_expected_result(board);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <test_name>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "flood_fill_stack") == 0) {
        test_flood_fill_stack();
    } else if (strcmp(argv[1], "flood_fill_queue") == 0) {
        test_flood_fill_queue();
    } else {
        fprintf(stderr, "unknown test: %s\n", argv[1]);
        return 1;
    }

    printf("PASS\n");
    return 0;
}
