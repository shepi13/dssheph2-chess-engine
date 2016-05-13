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

#define bPieces (bP | bR | bN | bB | bQ | bK)
#define wPieces (wP | wR | wN | wB | wQ | wK)

#define empty (~wPieces & ~bPieces)
#define Pieces (wPieces | bPieces)

int whiteKingside;
int whiteQueenside;
int blackKingside;
int blackQueenside;

int numMoves[2];

unsigned long long enpassantSquare;

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

//Using a linked list will be somewhat slower than other methods to check if a move is legal
//as it might rule out any possibility of some kind of binary search.
//However, when the engine has to loop through and try every move, it should be efficient,
//you are never accessing the last node of the list without accessing the one previous to it
//
//Might reconsider this structure later, as getting the memory required to store each new node
//could be expensive (malloc is relatively slow), and I am not sure that I want to
//risk storing a list that could use a decent amount of memory on the stack. 
//Discuss with advisor.
// Maybe implement static queue or use a dynamic array?
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

unsigned long long whitePawnRightAttacks(unsigned long long pawns);
unsigned long long whitePawnLeftAttacks(unsigned long long pawns);
unsigned long long whitePawnAttacks(unsigned long long pawns);
unsigned long long whitePawnCaptures(unsigned long long pawns);
unsigned long long whiteSinglePush(unsigned long long pawns);
unsigned long long whiteDoublePush(unsigned long long pawns);
unsigned long long whitePawnMoves(unsigned long long pawns);

unsigned long long blackPawnRightAttacks(unsigned long long pawns);
unsigned long long blackPawnLeftAttacks(unsigned long long pawns);
unsigned long long blackPawnAttacks(unsigned long long pawns);
unsigned long long blackPawnCaptures(unsigned long long pawns);
unsigned long long blackSinglePush(unsigned long long pawns);
unsigned long long blackDoublePush(unsigned long long pawns);
unsigned long long blackPawnMoves(unsigned long long pawns);

unsigned long long knightMoves(unsigned long long knights, int color);

unsigned long long kingMoves(unsigned long long kings, int color);

unsigned long long getRookMoveBitboard(int square, int color);

unsigned long long getBishopMoveBitboard(int square, int color);



int* getBits(unsigned long long bitboard);

void setRookMagic();
void generateRookMasks();
void generateRookBlockers();
void generateRookAttacks();
void generateRookMagic();
void generateRookMoveArray();

void setBishopMagic();
void generateBishopMasks();
void generateBishopBlockers();
void generateBishopAttacks();
void generateBishopMagic();
void generateBishopMoveArray();


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
