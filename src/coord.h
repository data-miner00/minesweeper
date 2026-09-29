#ifndef COORD_H
#define COORD_H

// A single (row, col) position on the board, used as the element type for
// the stack/queue-based flood fill variants.
typedef struct {
    int row;
    int col;
} Coord;

#endif // COORD_H
