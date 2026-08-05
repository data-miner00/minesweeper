#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

// Parses str as a base-10 int into *out. Returns false (and leaves *out
// untouched) if str has no digits at all, or has trailing junk after them.
static bool parse_int_arg(const char *str, int *out) {
    char *endptr;
    long value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        return false;
    }

    *out = (int)value;
    return true;
}

MinesweeperArgs parse_minesweeper_args(int argc, char *argv[]) {
    MinesweeperArgs args = {.size = 9, .numMines = 10};

    if (argc > 2) {
        if (strcmp(argv[1], "--size") == 0 || strcmp(argv[1], "-s") == 0) {
            if (!parse_int_arg(argv[2], &args.size)) {
                fprintf(stderr, "Invalid value for %s: %s\n", argv[1], argv[2]);
                exit(1);
            }
        } else if (strcmp(argv[1], "--mines") == 0 || strcmp(argv[1], "-m") == 0) {
            if (!parse_int_arg(argv[2], &args.numMines)) {
                fprintf(stderr, "Invalid value for %s: %s\n", argv[1], argv[2]);
                exit(1);
            }
        }
    }
    if (argc > 4) {
        if (strcmp(argv[3], "--size") == 0 || strcmp(argv[3], "-s") == 0) {
            if (!parse_int_arg(argv[4], &args.size)) {
                fprintf(stderr, "Invalid value for %s: %s\n", argv[3], argv[4]);
                exit(1);
            }
        } else if (strcmp(argv[3], "--mines") == 0 || strcmp(argv[3], "-m") == 0) {
            if (!parse_int_arg(argv[4], &args.numMines)) {
                fprintf(stderr, "Invalid value for %s: %s\n", argv[3], argv[4]);
                exit(1);
            }
        }
    }

    if (args.size <= 0) {
        fprintf(stderr, "Invalid size: %d (must be positive)\n", args.size);
        exit(1);
    }
    if (args.numMines < 0) {
        fprintf(stderr, "Invalid mines: %d (must be non-negative)\n", args.numMines);
        exit(1);
    }
    if (args.numMines >= args.size * args.size) {
        fprintf(stderr, "Too many mines: %d for a %dx%d board (must be less than %d)\n",
                args.numMines, args.size, args.size, args.size * args.size);
        exit(1);
    }

    return args;
}
