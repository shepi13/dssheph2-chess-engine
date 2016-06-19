#include "board.h"
#include "moves.h"
#include "magicNumbers.h"
#include "magicGeneration.h"
#include "moveGeneration.h"

int whiteKingside = 1;
int whiteQueenside = 1;
int blackKingside = 1;
int blackQueenside = 1;

int numMoves[2];

unsigned long long enpassantsquare = 0LL;

const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

int Moves_getEnd(Moves * m, size_t index)
{
    return m->move[index] & 0x3f;
}

int Moves_getStart(Moves * m, size_t index)
{
    return (m->move[index] >> 6) & 0x3f;
}

int Moves_getFlags(Moves * m, size_t index)
{
    return (m->move[index] >> 12) & 0x0f;
}

void Moves_setEnd(Moves * m, size_t index, unsigned int end)
{
    m->move[index] &= ~0x3f;
    m->move[index] |= end & 0x3f;
}

void Moves_setStart(Moves * m, size_t index, unsigned int start, int new)
{
    m->numMoves += new;
    m->move[index] &= ~0xfc0;
    m->move[index] |= (start & 0x3f) << 6;
}

void Moves_setFlags(Moves * m, size_t index, unsigned int flags)
{
    m->move[index] &= ~(0xf000);
    m->move[index] |= (flags & 0x0f) << 12;
}

//unsigned long long kingMoves(unsigned long long 
void printBoard()
{
    printf("\033[2J");

    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            char c = '-';
            int shift = x+8*(7-y);
            if((wK >> shift) % 2)
                c = 'K';
            else if((wQ >> shift) % 2)
                c = 'Q';
            else if((wN >> shift) % 2)
                c = 'N';
            else if((wP >> shift) % 2)
                c = 'P';
            else if((wR >> shift) % 2)
                c = 'R';
            else if((wB >> shift) % 2)
                c = 'B';
            else if((bK >> shift) % 2)
                c = 'k';
            else if((bQ >> shift) % 2)
                c = 'q';
            else if((bN >> shift) % 2)
                c = 'n';
            else if((bP >> shift) % 2)
                c = 'p';
            else if((bR >> shift) % 2)
                c = 'r';
            else if((bB >> shift) % 2)
                c = 'b';

            printf("\033[%d;%dH%c", y+1, 2*x+1, c);
        }
    }
    
    printf("\n");
}

void initializeBoard()
{
    srand(time(NULL));

    wK = 0x0000000000000010;
    wQ = 0x0000000000000008;
    wN = 0x0000000000000042;
    wP = 0x000000000000ff00;
    wR = 0x0000000000000081;
    wB = 0x0000000000000024;
    bK = 0x1000000000000000;
    bQ = 0x0800000000000000;
    bN = 0x4200000000000000;
    bP = 0x00ff000000000000;
    bR = 0x8100000000000000;
    bB = 0x2400000000000000;

    notHFile = 0x7f7f7f7f7f7f7f7f;
    notAFile = 0xfefefefefefefefe;

    notABFile = 0xfcfcfcfcfcfcfcfc;
    notGHFile = 0x3f3f3f3f3f3f3f3f;

    rank4 = 0x00000000ff000000;
    rank5 = 0x000000ff00000000;

    notRank8 = 0x00ffffffffffffff;
    notRank1 = 0xffffffffffffff00;

    generateRookMasks();
    generateRookBlockers();
    generateRookAttacks();
    setRookMagic();
    generateRookMoveArray();
    generateBishopMasks();
    generateBishopBlockers();
    generateBishopAttacks();
    setBishopMagic();
    generateBishopMoveArray();

}

void makeMoveFast(int start, int end, unsigned long long *piece)
{
    *piece &= 0xffffffffffffffffLL ^ (1LL << (long)start); 
    
    unsigned long long mask = ~(1LL << end);

    if(piece == &wK)
    {
        whiteKingside = 0;
        whiteQueenside = 0;
    }
    if(piece == &wR && start == 0)
        whiteQueenside = 0;
    if(piece == &wR && start == 7)
        whiteKingside = 0;

    if(piece == &bK)
    {
        blackKingside = 0;
        blackQueenside = 0;
    }
    if(start == 56)
        blackQueenside = 0;
    if(start == 63)
        blackKingside = 0;

    
    if(piece == &wP && (1LL << end) == enpassantsquare)
        bP &= ~(enpassantsquare) >> 8;
    else if(piece == &bP && (1LL << end) == enpassantsquare)
        wP &= ~(enpassantsquare << 8);


    if(piece == &wP && end == start+16)
        enpassantsquare = 1LL << (start+8);
    else if(piece == &bP && end == start-16)
        enpassantsquare = 1LL << (start-8);
    else
        enpassantsquare = 0; 

    wP &= mask;
    wR &= mask;
    wN &= mask;
    wB &= mask;
    wQ &= mask;
    wK &= mask;
    bP &= mask;
    bR &= mask;
    bN &= mask;
    bB &= mask;
    bQ &= mask;
    bK &= mask;

    *piece |= (1LL << (long)end);
}

void makeMove(int start, int end)
{
    unsigned long long startSquare = (1LL << start);

    if(wP & startSquare)
        makeMoveFast(start, end, &wP);
    else if(bP & startSquare)
        makeMoveFast(start, end, &bP);
    else if(wR & startSquare)
        makeMoveFast(start, end, &wR);
    else if(bR & startSquare)
        makeMoveFast(start, end, &bR);
    else if(wN & startSquare)
        makeMoveFast(start, end, &wN);
    else if(bN & startSquare)
        makeMoveFast(start, end, &bN);
    else if(wB & startSquare)
        makeMoveFast(start, end, &wB);
    else if(bB & startSquare)
        makeMoveFast(start, end, &bB);
    else if(wQ & startSquare)
        makeMoveFast(start, end, &wQ);
    else if(bQ & startSquare)
        makeMoveFast(start, end, &bQ);
    else if(wK & startSquare)
        makeMoveFast(start, end, &wK);
    else if(bK & startSquare)
        makeMoveFast(start, end, &bK);
}

void printBitBoard(unsigned long long board)
{
    printf("\033[2J");

    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            char c = '-';
            int shift = x+8*(7-y);
            if((board >> shift) % 2)
                c = '1';
            
            printf("\033[%d;%dH%c", y+1, 2*x+1, c);
        }
    }
    
    printf("\n");
}

//Return bit index of single set bit in bitboard
int bitIndex(unsigned long long bitboard)
{
    const unsigned long long debruijin = 0x03f79d71b4cb0a89;
    return index64[(bitboard * debruijin) >> 58];
}

void destroyBoard()
{
    for(int i = 0; i < 64; i++)
    {
        free(rookBlockers[i]);
        free(rookAttacks[i]);
        free(bishopBlockers[i]);
        free(bishopAttacks[i]);
        free(rookMoves[i]);
    }
}
