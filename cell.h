#ifndef CELL_H
#define CELL_H
#include <stdbool.h>

typedef struct {
    bool is_mine;
    int adjacent_count;
    bool is_revealed;
    bool is_flagged;
} Cell;

#endif // CELL_H
