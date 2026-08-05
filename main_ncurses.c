#include <ncurses.h>
#include "cell.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>

void draw_board(int size, Cell board[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col].is_revealed) {
                mvprintw(row, col * 2, "%d ", board[row][col].adjacent_count);
            } else if (board[row][col].is_flagged) {
                mvprintw(row, col * 2, "F ");
            } else {
                mvprintw(row, col * 2, ". ");
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

    int row, col;
    char cmd;

    initscr();            // enter curses mode, takes over the terminal
    cbreak();              // read input char-by-char, don't wait for Enter
    noecho();              // don't auto-print typed characters
    keypad(stdscr, TRUE);  // let getch() return arrow keys etc. as single KEY_* constants
    curs_set(1);           // hide the terminal's blinking cursor (optional, but cleaner once have cursor highlight)
    srand(time(NULL));

    bool is_running = true;

    while (is_running) {
        clear();
        draw_board(9, board);

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
            case 'q':
                is_running = false;
                break;
            default:
                break;
        }

        move(row, col * 2);
    }

    // if below is not run, can run `reset` or `stty sane` to reset terminal
    endwin();
    return 0;
}
