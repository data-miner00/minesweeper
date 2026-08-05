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

A CTest suite covers the shared argv-parsing logic: `tests/test_args.c` is a thin wrapper around `parse_minesweeper_args` (see `args.h`/`args.c` in Architecture below), and `tests/CMakeLists.txt` registers four subprocess-based cases — missing value, garbage value, trailing junk, valid input — via `add_test`. Subprocess-based rather than in-process because the parser calls `exit(1)` directly on bad input, so a failure can't be observed without killing the test runner itself. Run via `ctest --output-on-failure` from `build/` (or `make test`). No linter is configured.

## Architecture

The project builds **two executables from shared game logic**:
- `Minesweeper` (`main.c` + `board.c` + `args.c`) — the original console frontend, `scanf`-driven.
- `MinesweeperNcurses` (`main_ncurses.c` + `board.c` + `args.c`) — the ncurses frontend: arrow-key cursor movement, instant reveal/flag, an `A_REVERSE`-highlighted cursor cell stacked on top of the real terminal cursor (`curs_set(1)` + `move()`) — kept deliberately redundant so both cursor techniques stay visible for comparison — and win/loss end screens.

`CMakeLists.txt` defines both targets (each built from its own frontend `.c` plus the shared `board.c`/`args.c`), adds `include/` to both include paths, links `MinesweeperNcurses` against `find_package(Curses REQUIRED)`, and calls `enable_testing()` + `add_subdirectory(tests)` to wire up the CTest suite (see Build & run above). `include/shaun.h` is a leftover placeholder header (currently just `int age = 28;`), not used by any real logic yet.

Source layout:
- `cell.h` — the `Cell` struct (see below). Included by both frontends and by `board.c`.
- `board.h`/`board.c` — the shared, **UI-agnostic engine**: `init_board`, `place_mine`, `compute_counts`, `flood_fill`, `is_win`, `is_lose`. No I/O of any kind lives here by design, so both frontends can link it unchanged. If you're adding new game-logic (not rendering/input), it belongs here.
- `args.h`/`args.c` — shared argv parsing: `parse_minesweeper_args(argc, argv)` returns a `MinesweeperArgs { int size; int numMines; }`, used identically by both frontends' `main`. Rejects malformed `--size`/`-s`/`--mines`/`-m` values (via `strtol` + `endptr` checks) with an error message and `exit(1)`; a flag with a missing value is silently ignored (falls back to the default) rather than crashing.
- `main.c` — console frontend: owns `print_board` (`printf`-based rendering, not shared) plus the `scanf` REPL loop.
- `main_ncurses.c` — ncurses frontend: owns `draw_board` (`mvprintw`-based rendering, including the cursor highlight) and the `getch()`-driven input loop; touches `board.c`'s logic only through its existing function signatures.
- `tests/` — `test_args.c` is a thin wrapper around `parse_minesweeper_args` that prints `size=%d mines=%d`; `tests/CMakeLists.txt` builds it and registers four CTest cases against it as subprocesses (see Build & run above) — subprocess-based because the parser's `exit(1)` on bad input can't be observed in-process.

Game state is a single `Cell board[BOARD_SIZE][BOARD_SIZE]`, where `Cell` (in `cell.h`) is:

```c
typedef struct {
    bool is_mine;
    int adjacent_count;
    bool is_revealed;
    bool is_flagged;
} Cell;
```

(This replaced an earlier design of four parallel arrays — `board`/`counts`/`revealed`/`flagged` — indexed by the same `[row][col]`; if you see references to that shape, they're stale.) Board size is **dynamic**, not a `#define`: every `board.c` function takes a leading `int size` parameter — `Cell board[][size]` — and both frontends' `main` call the shared `parse_minesweeper_args` to get `size`/`numMines` from `argv` (`--size`/`-s`, `--mines`/`-m`, defaulting to 9/10), then declare `Cell board[size][size];` as a stack VLA. A VLA dimension can only reference an earlier parameter, which is why `size` must come before the array parameter in every signature.

Control flow in the console `main()` (`main.c`):
1. Parse `size`/`numMines` via `parse_minesweeper_args`; seed RNG; build the board: `init_board` (zeroes/falses every field) → `place_mine` (sets `.is_mine`) → `compute_counts` (sets `.adjacent_count`, `-1` for mine cells).
2. Run a REPL-style `while (true)` loop reading a command line as `scanf(" %c %d %d", &cmd, &row, &col)` — the leading space in the format string is required to skip a leftover newline before `%c`.
3. `'r'` reveals a cell via `flood_fill` (recursive cascade through connected zero-count cells; guards against re-visiting `is_revealed`/`is_flagged` cells to avoid infinite recursion), then checks for a loss (`board[row][col].is_mine`) or win (`is_win`).
4. `'f'` toggles `.is_flagged` for that cell without revealing anything.
5. `'q'` exits the loop.

Control flow in the ncurses `main()` (`main_ncurses.c`):
1. Same `parse_minesweeper_args` → `init_board`/`place_mine`/`compute_counts` setup, then the standard ncurses init sequence (`initscr()` → `cbreak()` → `noecho()` → `keypad(stdscr, TRUE)` → `curs_set(1)`).
2. Main loop condition is `is_running && !is_win(...) && !is_lose(...)`: each iteration calls `clear()`, `draw_board` (cursor-highlighted, non-reveal-all view), positions the real cursor with `move(row, col * 2)`, `refresh()`s, then blocks on `getch()`.
3. Arrow keys (`KEY_UP`/`KEY_DOWN`/`KEY_LEFT`/`KEY_RIGHT`) move `row`/`col`, clamped to `[0, size)`.
4. `'r'` calls `flood_fill` directly, after inline `is_revealed`/`is_flagged` guards (redundant with `flood_fill`'s own); unlike `main.c`, there's no explicit `board[row][col].is_mine` check here — a mine reveal is instead caught by the outer loop's `!is_lose(...)` condition on the next iteration. `'f'` toggles `.is_flagged`; `'q'` sets `is_running = false`.
5. After the loop, `is_win`/`is_lose` pick which end screen prints (message row `size + 2`, below the board); the loss screen redraws with `draw_board(size, -1, -1, board, true)` — `-1, -1` is a sentinel meaning "no cell is the cursor," since a real cursor position doesn't make sense on a reveal-all screen.
