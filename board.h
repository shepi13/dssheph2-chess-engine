#ifndef BOARD_H
#define BOARD_H

#define _GNU_SOURCE 1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "moves.h"
#include "magicNumbers.h"
#include "magicGeneration.h"

#define bPieces (bP | bR | bN | bB | bQ | bK)
#define wPieces (wP | wR | wN | wB | wQ | wK)

#define empty (~wPieces & ~bPieces)
#define Pieces (wPieces | bPieces)

int whiteKingside;
int whiteQueenside;
int blackKingside;
int blackQueenside;

int numMoves[2];

unsigned long long enpassantsquare;

unsigned long long wK, wQ, wB, wN, wR, wP, bK, bQ, bB, bN, bR, bP;
unsigned long long notAFile, notHFile;
unsigned long long rank4, rank5;
unsigned long long notABFile, notGHFile;
unsigned long long notRank8, notRank1;
unsigned long long wK, wQ, wB, wN, wR, wP, bK, bQ, bB, bN, bR, bP;
unsigned long long notAFile, notHFile;
unsigned long long rank4, rank5;
unsigned long long notABFile, notGHFile;
unsigned long long notRank8, notRank1;

//Move Databases
unsigned long long * rookMoves[64];
unsigned long long * bishopMoves[64];

//Magic numbers
unsigned long long rookMagic[64];
unsigned long long bishopMagic[64];

//Masks to store the possible blockers for bishops/rooks
unsigned long long bishopMask[64], rookMask[64];

//How many bits to shift to generate index - is same as 64-bitsSet
int magicShiftRooks[64], magicShiftBishops[64];

//A dynamic array to store variations for each piece on each square.
unsigned long long *bishopBlockers[64]; 
unsigned long long *rookBlockers[64];

unsigned long long *rookAttacks[64];
unsigned long long *bishopAttacks[64];

//Array to store number of set bits.
int bitsSetBishop[64]; 
int bitsSetRook[64];

//DeBruijin sequence used to calculate square number of single set bit in bitboard.
const int index64[64];

typedef struct my_move_list
{
    unsigned long long move[200];
    size_t numMoves;
} Moves;

int numfree;

int Moves_getEnd(Moves * m, size_t index);
int Moves_getStart(Moves * m, size_t index);
int Moves_getFlags(Moves * m, size_t index);
void Moves_setEnd(Moves * m, size_t index, unsigned int end);
void Moves_setStart(Moves * m, size_t index, unsigned int start, int new);
void Moves_setFlags(Moves * m, size_t index, unsigned int flags);

void printBoard();
void initializeBoard();
void makeMoveFast(int start, int end, unsigned long long *piece);
void makeMove(int start, int end);
void printBitBoard(unsigned long long board);
void destroyBoard();


int bitIndex(unsigned long long bitboard);
Moves * generatePawnMoves(Moves *m, int index, int color);
Moves * generateKnightMoves(Moves *m, int index, int color);
Moves * generateKingMoves(Moves *m, int index, int color);
Moves * generateRookMoves(Moves *m, int index, int color);
Moves * generateBishopMoves(Moves *m, int index, int color);
Moves * generateQueenMoves(Moves *m, int index, int color);
Moves * generateLegalMoves(int color);

int __builtin_popcountll (unsigned long long x);

#endif
