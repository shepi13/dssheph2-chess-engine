#include "moveGeneration.h"

//Color == 1 if white
Moves * generatePawnMoves(Moves *m, int index, int color)
{
    if(m == NULL)
        m = malloc(sizeof(Moves)); 
    
    unsigned long long temp = color == 1 ? wP : bP;

    while(temp)
    {
        
        unsigned long long piece = temp & -temp;    //Get LSB;
        unsigned long long legalMoves = 0;        

        if(color == 1)
            legalMoves = whitePawnMoves(piece);
        else
            legalMoves = blackPawnMoves(piece);


        while(legalMoves)
        {
            int end = bitIndex(legalMoves & -legalMoves);
            int start = bitIndex(piece);
            
            //If not promoting.
            if(end < 56 && color== 1 || end >= 8 && color != 1)
            {
                Moves_setStart(m, index, start, 1);
                Moves_setEnd(m, index, end);
                Moves_setFlags(m, index, 0);
                index++;
                numMoves[color]++;
            }
            else
            {
                for(int flag = 1; flag < 5; flag++)
                {
                    Moves_setStart(m, index, start, 1);
                    Moves_setEnd(m, index, end);
                    Moves_setFlags(m, index, flag);
                    index++;
                    numMoves[color]++;
                }
            }
    
            legalMoves = legalMoves & legalMoves-1;
        }
        
        temp = temp & temp-1;       //PopLSB.
    }

    return m;
}

Moves * generateKnightMoves(Moves *m, int index, int color)
{
    if(m == NULL)
        m = malloc(sizeof(Moves));

    unsigned long long temp = color == 1 ? wN : bN;

    while(temp)
    {

        unsigned long long piece = temp & -temp;    //Get LSB;
        unsigned long long legalMoves = knightMoves(piece, color);

        while(legalMoves)
        {
            Moves_setStart(m, index, bitIndex(piece), 1);
            Moves_setEnd(m, index, bitIndex(legalMoves & -legalMoves));
            Moves_setFlags(m, index, 0);

            index++;

            legalMoves = legalMoves & legalMoves-1;

            numMoves[color]++;
        }

        temp = temp & temp-1;       //PopLSB.
    }

    return m;
}

Moves * generateKingMoves(Moves *m, int index, int color)
{
    if(m == NULL)
        m = malloc(sizeof(Moves));

    unsigned long long temp = color == 1 ? wK : bK;

    while(temp)
    {

        unsigned long long piece = temp & -temp;    //Get LSB;
        unsigned long long legalMoves = kingMoves(piece, color);

        while(legalMoves)
        {
            Moves_setStart(m, index, bitIndex(piece), 1);
            Moves_setEnd(m, index, bitIndex(legalMoves & -legalMoves));
            Moves_setFlags(m, index, 0);

            index++;

            legalMoves = legalMoves & legalMoves-1;

            numMoves[color]++;
        }

        temp = temp & temp-1;       //PopLSB.
    }

    return m;
}

Moves * generateRookMoves(Moves *m, int index, int color)
{
    if(m == NULL)
        m = malloc(sizeof(Moves));

    unsigned long long temp = color == 1 ? wR : bR;

    while(temp)
    {
        unsigned long long piece = temp & -temp;    //Get LSB;
        int square = bitIndex(piece);
        unsigned long long legalMoves = getRookMoveBitboard(square, color);

        while(legalMoves)
        {
            Moves_setStart(m, index, square, 1);
            Moves_setEnd(m, index, bitIndex(legalMoves & -legalMoves));
            Moves_setFlags(m, index, 0);

            index++;

            legalMoves = legalMoves & legalMoves-1;
            numMoves[color]++;
        }

        temp = temp & temp-1;       //PopLSB.
    }

    return m;
}

Moves * generateBishopMoves(Moves *m, int index, int color)
{
    if(m == NULL)
        m = malloc(sizeof(Moves));

    unsigned long long temp = color == 1 ? wB : bB;

    while(temp)
    {

        unsigned long long piece = temp & -temp;    //Get LSB;
        int square = bitIndex(piece);
        unsigned long long legalMoves = getBishopMoveBitboard(square, color);

        while(legalMoves)
        {
            Moves_setStart(m, index, square, 1);
            Moves_setEnd(m, index, bitIndex(legalMoves & -legalMoves));
            Moves_setFlags(m, index, 0);

            index++;

            legalMoves = legalMoves & legalMoves-1;
            numMoves[color]++;
        }

        temp = temp & temp-1;       //PopLSB.
    }

    return m;
}

Moves * generateQueenMoves(Moves *m, int index, int color)
{
    if(m == NULL)
        m = malloc(sizeof(Moves));
    
    unsigned long long temp = color == 1 ? wQ : bQ;

    while(temp)
    {

        unsigned long long piece = temp & -temp;    //Get LSB;
        int square = bitIndex(piece);
        unsigned long long rookMoves = getRookMoveBitboard(square, color);
        unsigned long long bishopMoves = getBishopMoveBitboard(square, color);
        unsigned long long legalMoves = rookMoves | bishopMoves;

        while(legalMoves)
        {
            Moves_setStart(m, index, square, 1);
            Moves_setEnd(m, index, bitIndex(legalMoves & -legalMoves));
            Moves_setFlags(m, index, 0);

            index++;

            legalMoves = legalMoves & legalMoves-1;
            numMoves[color]++;
        }

        temp = temp & temp-1;       //PopLSB.
    }

    return m;
}

Moves * generateLegalMoves(int color)
{
    numMoves[color] = 0;

    Moves * m = malloc(sizeof(Moves));
    m->numMoves = 0;
    m = generatePawnMoves(m, 0, color);
    m = generateKnightMoves(m, m->numMoves, color);
    m = generateKingMoves(m, m->numMoves, color); 
    m = generateRookMoves(m, m->numMoves, color);
    m = generateBishopMoves(m, m->numMoves, color);
    m = generateQueenMoves(m, m->numMoves, color);
    return m;
}
