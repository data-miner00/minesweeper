#include <stdlib.h>
#include <string.h>
#include "args.h"

MinesweeperArgs parse_minesweeper_args(int argc, char* argv[]) {
    MinesweeperArgs args = { .size = 9, .numMines = 10 };

    if (argc > 1) {
        if (strcmp(argv[1], "--size") == 0 || strcmp(argv[1], "-s") == 0) {
            args.size = atoi(argv[2]);
        } else if (strcmp(argv[1], "--mines") == 0 || strcmp(argv[1], "-m") == 0) {
            args.numMines = atoi(argv[2]);
        }
    }
    if (argc > 3) {
        if (strcmp(argv[3], "--size") == 0 || strcmp(argv[3], "-s") == 0) {
            args.size = atoi(argv[4]);
        } else if (strcmp(argv[3], "--mines") == 0 || strcmp(argv[3], "-m") == 0) {
            args.numMines = atoi(argv[4]);
        }
    }

    return args;
}
