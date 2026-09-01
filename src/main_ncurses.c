#include <ncurses.h>
#include "cell.h"
#include "board.h"
#include "args.h"
#include <time.h>
#include <stdlib.h>

enum {
    PAIR_COUNT_1 = 1,
    PAIR_COUNT_2,
    PAIR_COUNT_3,
    PAIR_COUNT_4,
    PAIR_COUNT_5,
    PAIR_COUNT_6,
    PAIR_COUNT_7,
    PAIR_COUNT_8,
    PAIR_FLAG,
    PAIR_MINE,
};

void draw_board(int size, int cursorRow, int cursorCol, Cell board[][size], bool is_reveal_all);

int main(int argc, char *argv[]) {
    MinesweeperArgs args = parse_minesweeper_args(argc, argv);
    int size = args.size;
    int numMines = args.numMines;

    srand(time(NULL));

    Cell board[size][size];
    init_board(size, board);

    int row = 0, col = 0, total_flagged = 0;

    initscr();            // enter curses mode, takes over the terminal
    cbreak();             // read input char-by-char, don't wait for Enter
    noecho();             // don't auto-print typed characters
    keypad(stdscr, TRUE); // let getch() return arrow keys etc. as single KEY_* constants
    curs_set(1);          // primary cursor
    start_color();        // enable colors
    init_pair(PAIR_COUNT_1, COLOR_BLUE, COLOR_BLACK);
    init_pair(PAIR_COUNT_2, COLOR_GREEN, COLOR_BLACK);
    init_pair(PAIR_COUNT_3, COLOR_RED, COLOR_BLACK);
    init_pair(PAIR_COUNT_4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(PAIR_COUNT_5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PAIR_COUNT_6, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_COUNT_7, COLOR_WHITE, COLOR_BLACK);
    init_pair(PAIR_COUNT_8, COLOR_WHITE, COLOR_BLACK);
    init_pair(PAIR_FLAG, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PAIR_MINE, COLOR_RED, COLOR_BLACK);

    bool is_running = true;
    bool is_materialized = false;

    while (is_running && !is_win(size, board) && !is_lose(size, board)) {
        clear();
        draw_board(size, row, col, board, false);

        mvprintw(0, size * 2 + 2, "Total flagged: %d", total_flagged);

        move(row, col * 2);
        refresh();
        int ch = getch();

        switch (ch) {
        case KEY_UP:
        case 'k':
            if (row > 0)
                row--;
            break;
        case KEY_DOWN:
        case 'j':
            if (row < size - 1)
                row++;
            break;
        case KEY_LEFT:
        case 'h':
            if (col > 0)
                col--;
            break;
        case KEY_RIGHT:
        case 'l':
            if (col < size - 1)
                col++;
            break;
        case 'r':
            if (!is_materialized) {
                place_mine(size, numMines, row, col, board);
                compute_counts(size, board);

                is_materialized = true;
            }

            if (board[row][col].is_revealed) {
                // you have already revealed this cell
            } else if (board[row][col].is_flagged) {
                // you have already flagged this cell
            } else {
                flood_fill(size, board, row, col);
            }
            break;
        case 'f':
            if (board[row][col].is_revealed) {
                // you have already revealed this cell
            } else {
                if (board[row][col].is_flagged) {
                    total_flagged--;
                    board[row][col].is_flagged = false;
                } else {
                    total_flagged++;
                    board[row][col].is_flagged = true;
                }
            }
            break;
        case 'q':
            is_running = false;
            break;
        default:
            break;
        }
    }

    if (is_win(size, board)) {
        clear();
        draw_board(size, row, col, board, false);
        mvprintw(size + 2, 0, "You have won!\n");
        refresh();
        getch();
    } else if (is_lose(size, board)) {
        mvprintw(size + 2, 0, "You have lost!\n");
        draw_board(size, -1, -1, board, true);
        refresh();
        getch();
    }

    // if below is not run, can run `reset` or `stty sane` to reset terminal
    endwin();
    return 0;
}

void draw_board(int size, int cursorRow, int cursorCol, Cell board[][size], bool is_reveal_all) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            // Secondary cursor: Turn on because ugly
            // if (row == cursorRow && col == cursorCol) {
            //     attron(A_REVERSE);
            // }

            if (is_reveal_all) {
                if (board[row][col].is_mine) {
                    attron(COLOR_PAIR(PAIR_MINE));
                    mvprintw(row, col * 2, "* ");
                    attroff(COLOR_PAIR(PAIR_MINE));
                } else {
                    int count = board[row][col].adjacent_count;
                    attron(COLOR_PAIR(count));
                    mvprintw(row, col * 2, "%d ", count);
                    attroff(COLOR_PAIR(count));
                }
            } else {
                if (board[row][col].is_revealed) {
                    if (board[row][col].is_mine) {
                        attron(COLOR_PAIR(PAIR_MINE));
                        mvprintw(row, col * 2, "* ");
                        attroff(COLOR_PAIR(PAIR_MINE));
                    } else {
                        int count = board[row][col].adjacent_count;
                        attron(COLOR_PAIR(count));
                        mvprintw(row, col * 2, "%d ", count);
                        attroff(COLOR_PAIR(count));
                    }
                } else if (board[row][col].is_flagged) {
                    attron(COLOR_PAIR(PAIR_FLAG));
                    mvprintw(row, col * 2, "F ");
                    attroff(COLOR_PAIR(PAIR_FLAG));
                } else {
                    mvprintw(row, col * 2, ". ");
                }
            }

            // Secondary cursor: Turn off because ugly
            // if (row == cursorRow && col == cursorCol) {
            //     attroff(A_REVERSE);
            // }
        }

        mvprintw(row, size * 2, "\n");
        // mvprintw(row, size * 2, "%d\n", row);
    }
}
