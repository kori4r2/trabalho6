#ifndef _CHESS_H_
#define _CHESS_H_

#define W_ROOK 'R'
#define W_KNIGHT 'N'
#define W_BISHOP 'B'
#define W_QUEEN 'Q'
#define W_KING 'K'
#define W_PAWN 'P'
#define B_ROOK 'r'
#define B_KNIGHT 'n'
#define B_BISHOP 'b'
#define B_QUEEN 'q'
#define B_KING 'k'
#define B_PAWN 'p'
#define WHITES_TURN 'w'
#define BLACKS_TURN 'b'

#include "item.h"
#include "queue.h"

typedef struct piece PIECE;
typedef struct table TABLE;

int read_table(FILE*, TABLE*);


#endif
