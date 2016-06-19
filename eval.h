//The actual chess engine.
//TODO:
//add piece-square tables, try to optimize engine to get to depth 8 goal
//
//consider trying to learn how to implement a hash table for transpositions.
#ifndef EVAL_H
#define EVAL_H

#include "board.h"

unsigned long long bestMove;

int popCount(unsigned long long bitboard);
int eval();
int alphaBetaSearch(int side, int currentDepth, int alpha, int beta);

#endif

