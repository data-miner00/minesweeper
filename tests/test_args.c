#include <stdio.h>
#include "args.h"

// Thin CLI wrapper around parse_minesweeper_args so CTest can drive it as a
// subprocess per case and assert on exit code / printed values.
int main(int argc, char* argv[]) {
    MinesweeperArgs args = parse_minesweeper_args(argc, argv);
    printf("size=%d mines=%d\n", args.size, args.numMines);
    return 0;
}
