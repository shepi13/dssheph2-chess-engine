//Functions for move generation
#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include "board.h"
#include "moves.h"

Moves * generatePawnMoves(Moves *m, int index, int color);
Moves * generateKnightMoves(Moves *m, int index, int color);
Moves * generateKingMoves(Moves *m, int index, int color);
Moves * generateRookMoves(Moves *m, int index, int color);
Moves * generateBishopMoves(Moves *m, int index, int color);
Moves * generateQueenMoves(Moves *m, int index, int color);
Moves * generateLegalMoves(int color);

#endif
