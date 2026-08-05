#include <ncurses.h>
#include "cell.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>

void draw_board(int size, int cursorRow, int cursorCol, Cell board[][size], bool is_reveal_all) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            // Secondary cursor
            if (row == cursorRow && col == cursorCol) {
                attron(A_REVERSE);
            }

            if (is_reveal_all) {
                if (board[row][col].is_mine) {
                    mvprintw(row, col * 2, "* ");
                } else {
                    mvprintw(row, col * 2, "%d ", board[row][col].adjacent_count);
                }
            } else {
                if (board[row][col].is_revealed) {
                    if (board[row][col].is_mine) {
                        mvprintw(row, col * 2, "* ");
                    } else {
                        mvprintw(row, col * 2, "%d ", board[row][col].adjacent_count);
                    }
                } else if (board[row][col].is_flagged) {
                    mvprintw(row, col * 2, "F ");
                } else {
                    mvprintw(row, col * 2, ". ");
                }
            }

            // Secondary cursor
            if (row == cursorRow && col == cursorCol) {
                attroff(A_REVERSE);
            }
        }

        mvprintw(row, size * 2, "\n");
        // mvprintw(row, size * 2, "%d\n", row);
    }
}

int main(void) {

    Cell board[9][9];
    init_board(9, board);
    place_mine(9, 10, board);
    compute_counts(9, board);

    int row = 0, col = 0;

    initscr();            // enter curses mode, takes over the terminal
    cbreak();              // read input char-by-char, don't wait for Enter
    noecho();              // don't auto-print typed characters
    keypad(stdscr, TRUE);  // let getch() return arrow keys etc. as single KEY_* constants
    curs_set(1);           // primary cursor
    srand(time(NULL));

    bool is_running = true;

    while (is_running && !is_win(9, board) && !is_lose(9, board)) {
        clear();
        draw_board(9, row, col, board, false);

        move(row, col * 2);

        refresh();
        int ch = getch();

        switch (ch) {
            case KEY_UP:
                if (row > 0) row--;
                break;
            case KEY_DOWN:
                if (row < 8) row++;
                break;
            case KEY_LEFT:
                if (col > 0) col--;
                break;
            case KEY_RIGHT:
                if (col < 8) col++;
                break;
            case 'r':
                if (board[row][col].is_revealed) {
                    // you have already revealed this cell
                } else if (board[row][col].is_flagged) {
                    // you have already flagged this cell
                } else {
                    flood_fill(9, board, row, col);
                }
                break;
            case 'f':
                if (board[row][col].is_revealed) {
                    // you have already revealed this cell
                } else {
                    // toggle flag
                    board[row][col].is_flagged = !board[row][col].is_flagged;
                }
                break;
            case 'q':
                is_running = false;
                break;
            default:
                break;
        }
    }

    if (is_win(9, board)) {
        mvprintw(11, 0, "You have won!\n");
        refresh();
        getch();
    } else if (is_lose(9, board)) {
        mvprintw(11, 0, "You have lost!\n");
        draw_board(9, 12, 0, board, true);
        refresh();
        getch();
    }

    // if below is not run, can run `reset` or `stty sane` to reset terminal
    endwin();
    return 0;
}
