# basically a command runner
.PHONY: build run run-ncurses rn test format

build: build/Makefile
	cmake --build build

build/Makefile:
	cmake -S . -B build

run: build
	./build/Minesweeper

run-ncurses: build
	./build/MinesweeperNcurses

rn: run-ncurses

test: build
	cd build && ctest --output-on-failure

format:
	find . -path ./build -prune -o \( -name '*.c' -o -name '*.h' \) -print | xargs clang-format -i
