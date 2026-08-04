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

After the initial `cmake ..` configure, `cmake --build .` from `build/` rebuilds both executables (see below) after editing sources.

There is no test suite or linter configured in this repository.

## Architecture

The project builds **two executables from shared game logic**:
- `Minesweeper` (`main.c` + `board.c`) — the original console frontend, `scanf`-driven.
- `MinesweeperNcurses` (`main_ncurses.c` + `board.c`) — an in-progress ncurses frontend. Currently just an `initscr()`/`endwin()` skeleton; the ncurses render/input loop hasn't been written yet.

`CMakeLists.txt` defines both targets, adds `include/` to both include paths, and links `MinesweeperNcurses` against `find_package(Curses REQUIRED)`. `include/shaun.h` is a leftover placeholder header (currently just `int age = 28;`), not used by any real logic yet.

Source layout:
- `cell.h` — the `Cell` struct (see below). Included by both frontends and by `board.c`.
- `board.h`/`board.c` — the shared, **UI-agnostic engine**: `init_board`, `place_mine`, `compute_counts`, `flood_fill`, `is_win`. No I/O of any kind lives here by design, so both frontends can link it unchanged. If you're adding new game-logic (not rendering/input), it belongs here.
- `main.c` — console frontend: owns `print_board` (`printf`-based rendering, not shared) plus the `scanf` REPL loop.
- `main_ncurses.c` — ncurses frontend: will own its own rendering (e.g. `mvprintw`/`addch`) and input handling (`getch`) once written; must not touch `board.c`'s logic directly except through its existing function signatures.

Game state is a single `Cell board[BOARD_SIZE][BOARD_SIZE]`, where `Cell` (in `cell.h`) is:

```c
typedef struct {
    bool is_mine;
    int adjacent_count;
    bool is_revealed;
    bool is_flagged;
} Cell;
```

(This replaced an earlier design of four parallel arrays — `board`/`counts`/`revealed`/`flagged` — indexed by the same `[row][col]`; if you see references to that shape, they're stale.) Board size is **dynamic**, not a `#define`: every `board.c` function takes a leading `int size` parameter — `Cell board[][size]` — and `size`/`numMines` are parsed from `argv` in `main` (`--size`/`-s`, `--mines`/`-m`, defaulting to 9/10). A VLA dimension can only reference an earlier parameter, which is why `size` must come before the array parameter in every signature. `main` declares `Cell board[size][size];` as a stack VLA.

Control flow in the console `main()` (`main.c`):
1. Parse `size`/`numMines` from `argv`; seed RNG; build the board: `init_board` (zeroes/falses every field) → `place_mine` (sets `.is_mine`) → `compute_counts` (sets `.adjacent_count`, `-1` for mine cells).
2. Run a REPL-style `while (true)` loop reading a command line as `scanf(" %c %d %d", &cmd, &row, &col)` — the leading space in the format string is required to skip a leftover newline before `%c`.
3. `'r'` reveals a cell via `flood_fill` (recursive cascade through connected zero-count cells; guards against re-visiting `is_revealed`/`is_flagged` cells to avoid infinite recursion), then checks for a loss (`board[row][col].is_mine`) or win (`is_win`).
4. `'f'` toggles `.is_flagged` for that cell without revealing anything.
5. `'q'` exits the loop.

Known unfixed bugs in `main.c`'s `argv` parsing (missing-value crash, `atoi` silent-failure on garbage input, no check that `numMines < size*size`): tracked in `PROGRESS.md`'s backlog, not yet fixed.
