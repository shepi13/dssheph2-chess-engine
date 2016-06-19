//Contains functions used to compute possible moves for each piece.
#ifndef MOVES_H
#define MOVES_H

#include "board.h"

//Functions for pawn moves
//TODO:
//Merge into only a couple functions, this many one liners all calling
//each other is ridiculous.

//Probably keep pawn attacks, pawn captures, and pawn moves. Maybe keep
//double push in case it helps with enpassant. Get rid of right attacks
//and left attacks for sure.
unsigned long long whitePawnRightAttacks(unsigned long long pawns);
unsigned long long whitePawnLeftAttacks(unsigned long long pawns);    
unsigned long long whitePawnAttacks(unsigned long long pawns);
unsigned long long whitePawnCaptures(unsigned long long pawns);
unsigned long long whiteSinglePush(unsigned long long pawns);
unsigned long long whiteDoublePush(unsigned long long pawns);
unsigned long long whitePawnMoves(unsigned long long pawns);

//Black pawns
//TODO:
//Merge black pawn moves into same functions as white pawn moves
unsigned long long blackPawnRightAttacks(unsigned long long pawns);
unsigned long long blackPawnLeftAttacks(unsigned long long pawns);
unsigned long long blackPawnAttacks(unsigned long long pawns);
unsigned long long blackPawnCaptures(unsigned long long pawns);
unsigned long long blackSinglePush(unsigned long long pawns);
unsigned long long blackDoublePush(unsigned long long pawns);
unsigned long long blackPawnMoves(unsigned long long pawns);

//Functions for knight moves
unsigned long long knightMoves(unsigned long long knights, int color);

//Functions for king moves
unsigned long long kingMoves(unsigned long long kings, int color);

//Functions for bishop moves
unsigned long long getBishopMoveBitboard(int square, int color);

//Functions for rook moves
unsigned long long getRookMoveBitboard(int square, int color);

#endif
