#ifndef BOARD_H
#define BOARD_H
#include "cell.h"

void init_board(int size, Cell board[][size]);
void place_mine(int size, int numMines, int safeRow, int safeCol, Cell board[][size]);
void compute_counts(int size, Cell board[][size]);
void flood_fill(int size, Cell board[][size], int row, int col);
void flood_fill_stack(int size, Cell board[][size], int row, int col);
void flood_fill_queue(int size, Cell board[][size], int row, int col);
bool is_win(int size, Cell board[][size]);
bool is_lose(int size, Cell board[][size]);

#endif // BOARD_H
