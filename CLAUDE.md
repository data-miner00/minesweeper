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

Game state is currently represented as several **parallel `BOARD_SIZE x BOARD_SIZE` arrays**, all indexed by the same `[row][col]`:
- `board` (`char`): `'.'` for empty, `'*'` for a mine.
- `counts` (`char`): `'*'` mirrors mine cells, otherwise `'0'`-`'8'` — the adjacent-mine count as a char digit.
- `revealed` (`bool`): whether the player has revealed that cell.
- `flagged` (`bool`): whether the player has flagged that cell as a suspected mine.

`BOARD_SIZE` (9) and `NUM_MINES` (10) are `#define`d at the top of `main.c`. Note: a struct-based refactor that folds these four arrays into a single `Cell board[BOARD_SIZE][BOARD_SIZE]` (with `is_mine`/`adjacent_count`/`is_revealed`/`is_flagged` fields per cell) is planned/in progress — check whether `main.c` still uses the parallel-array form or has already moved to a `Cell` struct before assuming which one is current.

Control flow in `main()`:
1. Seed RNG, build the board: `init_board` → `place_mine` → `compute_counts`, then `init_bool_board` for `revealed` and `flagged`.
2. Run a REPL-style `while (true)` loop reading a command line as `scanf(" %c %d %d", &cmd, &row, &col)` — the leading space in the format string is required to skip a leftover newline before `%c`.
3. `'r'` reveals a cell via `flood_fill` (recursive cascade through connected zero-count cells; guards against re-visiting `revealed`/`flagged` cells to avoid infinite recursion), then checks for a loss (`board[row][col] == '*'`) or win (`is_win`).
4. `'f'` toggles `flagged` for that cell without revealing anything.
