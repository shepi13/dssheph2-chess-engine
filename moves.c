#include "moves.h"

unsigned long long whitePawnRightAttacks(unsigned long long pawns)
{
    return pawns << 9 & notAFile;
}

unsigned long long whitePawnLeftAttacks(unsigned long long pawns)
{
    return pawns << 7 & notHFile;
}

unsigned long long whitePawnAttacks(unsigned long long pawns)
{
    return whitePawnRightAttacks(pawns) | whitePawnLeftAttacks(pawns);
}

unsigned long long whitePawnCaptures(unsigned long long pawns)
{
    return whitePawnAttacks(pawns) & (bPieces | enpassantsquare);
}

unsigned long long whiteSinglePush(unsigned long long pawns)
{
    return (pawns << 8) & empty; 
}

unsigned long long whiteDoublePush(unsigned long long pawns)
{
    return (whiteSinglePush(pawns) << 8) & empty & rank4;
} 

unsigned long long whitePawnMoves(unsigned long long pawns)
{
    return whiteSinglePush(pawns) | whiteDoublePush(pawns) | whitePawnCaptures(pawns);
}

unsigned long long blackPawnRightAttacks(unsigned long long pawns)
{
    return (pawns >> 7) & notAFile;
}

unsigned long long blackPawnLeftAttacks(unsigned long long pawns)
{
    return (pawns >> 9) & notHFile;
}

unsigned long long blackPawnAttacks(unsigned long long pawns)
{
    return blackPawnRightAttacks(pawns) | blackPawnLeftAttacks(pawns);
}

unsigned long long blackPawnCaptures(unsigned long long pawns)
{
    return blackPawnAttacks(pawns) & (wPieces | enpassantsquare);
}

unsigned long long blackSinglePush(unsigned long long pawns)
{
    return (pawns >> 8) & empty;
}

unsigned long long blackDoublePush(unsigned long long pawns)
{
    return (blackSinglePush(pawns) >> 8) & empty & rank5;
}

unsigned long long blackPawnMoves(unsigned long long pawns)
{
    return blackSinglePush(pawns) | blackDoublePush(pawns) | blackPawnCaptures(pawns);
}

unsigned long long knightMoves(unsigned long long knights, int color)
{
    unsigned long long knightNNE = (knights << 17) & notAFile;
    unsigned long long knightNNW = (knights << 15) & notHFile;
    unsigned long long knightNEE = (knights << 10) & notABFile;
    unsigned long long knightNWW = (knights << 6) & notGHFile;
    unsigned long long knightSSE = (knights >> 15) & notAFile;
    unsigned long long knightSSW = (knights >> 17) & notHFile;
    unsigned long long knightSEE = (knights >> 6) & notABFile;
    unsigned long long knightSWW = (knights >> 10) & notGHFile;

    unsigned long long knightNMoves = knightNNE | knightNNW | knightNWW | knightNEE;
    unsigned long long knightSMoves = knightSSE | knightSSW | knightSWW | knightSEE;

    if(color == 1)
        return (knightNMoves | knightSMoves) & ~wPieces;
    else
        return (knightNMoves | knightSMoves) & ~bPieces;
}

unsigned long long kingMoves(unsigned long long kings, int color)
{
    unsigned long long kingW = (kings << 1) & notAFile;
    unsigned long long kingE = (kings >> 1) & notHFile;
    unsigned long long kingNW = (kings << 9) & notAFile;
    unsigned long long kingNE = (kings << 7) & notHFile;
    unsigned long long kingSE = (kings >> 9) & notHFile;
    unsigned long long kingSW = (kings >> 7) & notAFile;
    unsigned long long kingS = (kings >> 8);
    unsigned long long kingN = (kings << 8);

    unsigned long long kingSMoves = kingS | kingSE | kingSW | kingE;
    unsigned long long kingNMoves = kingN | kingNE | kingNW | kingW;

    if(color == 1)
        return (kingSMoves | kingNMoves) & ~wPieces;
    else
        return (kingSMoves | kingNMoves) & ~bPieces;
}

unsigned long long getRookMoveBitboard(int square, int color)
{
    unsigned long long pieces = bPieces | wPieces;
    unsigned long long index = rookMask[square] & pieces;

    int idx = (int)((index * rookMagic[square]) >> magicShiftRooks[square]);

    unsigned long long retval = rookMoves[square][idx];

    if(color == 1)
        return retval & ~wPieces;

    else
        return retval & ~bPieces;
} 

unsigned long long getBishopMoveBitboard(int square, int color)
{
    unsigned long long pieces = bPieces | wPieces;
    unsigned long long index = bishopMask[square] & pieces;

    int idx = (int)((index * bishopMagic[square]) >> magicShiftBishops[square]);
    unsigned long long retval = bishopMoves[square][idx];

    if(color == 1)
        return retval & ~wPieces;

    else
        return retval & ~bPieces;
}

