#ifndef MAGICGENERATION_H
#define MAGICGENERATION_H
#include "board.h"

void generateRookMasks();
void generateBishopMasks();

int* getBits(unsigned long long bitboard);

void generateRookBlockers();
void generateBishopBlockers();

void generateRookAttacks();
void generateBishopAttacks();

void generateRookMagic();
void generateBishopMagic();

void generateRookMoveArray();
void generateBishopMoveArray();

#endif
