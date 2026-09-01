#ifndef ARGS_H
#define ARGS_H

typedef struct {
    int size;
    int numMines;
} MinesweeperArgs;

MinesweeperArgs parse_minesweeper_args(int argc, char *argv[]);

#endif // ARGS_H
