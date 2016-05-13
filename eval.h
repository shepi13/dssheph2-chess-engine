#ifndef EVAL_H
#define EVAL_H

#include "board.h"

unsigned long long bestMove;

int popCount(unsigned long long bitboard);
int eval();
int alphaBetaSearch(int side, int currentDepth, int alpha, int beta);

#endif

