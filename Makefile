# basically a command runner
.PHONY: build run run-ncurses rn

build: build/Makefile
	cmake --build build

build/Makefile:
	cmake -S . -B build

run: build
	./build/Minesweeper

run-ncurses: build
	./build/MinesweeperNcurses

rn: run-ncurses
