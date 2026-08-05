# basically a command runner
.PHONY: build run run-ncurses rn test

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
