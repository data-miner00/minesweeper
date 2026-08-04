# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project context

This is a beginner's C learning project: a CLI Minesweeper game built incrementally, one small concept at a time (2D arrays → structs, `rand()`, recursion, `scanf`, etc.). The user is new to C.

When working in this repo, prefer explaining the relevant C concept and letting the user write the change themselves, then reviewing it, over writing large chunks of code unprompted. Keep changes scoped to the specific step being worked on rather than jumping ahead to later features (flagging, structs, ncurses, etc. are deliberately staged, not missing).

## Build & run

```
mkdir -p build && cd build
cmake ..
cmake --build .
./Minesweeper
```

After the initial `cmake ..` configure, `cmake --build .` from `build/` is sufficient to rebuild after editing `main.c`.

There is no test suite or linter configured in this repository.

## Architecture

Everything lives in a single `main.c`; there is no test suite or linter configured. `CMakeLists.txt` builds one executable (`Minesweeper`) from `main.c` and adds `include/` to the include path. `include/shaun.h` is a leftover placeholder header (currently just `int age = 28;`), not used by any real logic yet.

Game state is a single `Cell board[BOARD_SIZE][BOARD_SIZE]`, where `Cell` is:

```c
typedef struct {
    bool is_mine;
    int adjacent_count;
    bool is_revealed;
    bool is_flagged;
} Cell;
```

(This replaced an earlier design of four parallel arrays — `board`/`counts`/`revealed`/`flagged` — indexed by the same `[row][col]`; if you see references to that shape, they're stale.) Every board-manipulating function takes a single `Cell board[][BOARD_SIZE]` parameter: `init_board`, `place_mine`, `compute_counts`, `print_display`, `flood_fill`, `is_win`. `print_board` is kept as a separate function that always shows mines, used only for the full-board reveal on loss.

`BOARD_SIZE` (9) and `NUM_MINES` (10) are `#define`d at the top of `main.c`.

Control flow in `main()`:
1. Seed RNG, build the board: `init_board` (zeroes/falses every field) → `place_mine` (sets `.is_mine`) → `compute_counts` (sets `.adjacent_count`, `-1` for mine cells).
2. Run a REPL-style `while (true)` loop reading a command line as `scanf(" %c %d %d", &cmd, &row, &col)` — the leading space in the format string is required to skip a leftover newline before `%c`.
3. `'r'` reveals a cell via `flood_fill` (recursive cascade through connected zero-count cells; guards against re-visiting `is_revealed`/`is_flagged` cells to avoid infinite recursion), then checks for a loss (`board[row][col].is_mine`) or win (`is_win`).
4. `'f'` toggles `.is_flagged` for that cell without revealing anything.
