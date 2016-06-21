#include "eval.h"
#include "moveGeneration.h"

unsigned long long bestMove;

const int maxDepth = 6;
const int pawnVal = 100;
const int knightVal = 370;
const int bishopVal = 390;
const int rookVal = 550;
const int queenVal = 950;
const int kingVal = 10000;
const int mobilityVal = 10;

/*const int pawnEndgameTable[] = 
{
0, 0, 0, 0, 0, 0, 0, 0,
60, 55, 50, 50, 50, 50, 55, 60,
30, 20, 10, 10, 10, 10, 20, 30,
25, 20, 5, 5, 5, 5, 20, 27,
10, 10, 0, 0, 0, 0, 10, 10,
0, 0, 0, 0, 0, 0, 0, 0,
*/

int popCount(unsigned long long bitboard)
{
    int result = 0;

    while(bitboard)
    {
        result++;
        bitboard &= (bitboard-1);
    }

    return result;
}

int eval()
{
    numMoves[1] = getNumMoves(1);
    numMoves[0] = getNumMoves(0);

    return  pawnVal * (__builtin_popcountll(wP) - __builtin_popcountll(bP)) +
            knightVal * (__builtin_popcountll(wN) - __builtin_popcountll(bN)) + 
            bishopVal * (__builtin_popcountll(wB) - __builtin_popcountll(bB)) +
            rookVal * (__builtin_popcountll(wR) - __builtin_popcountll(bR)) + 
            queenVal * (__builtin_popcountll(wQ) - __builtin_popcountll(bQ)) +
            mobilityVal * (numMoves[1] - numMoves[0]);
}

int alphaBetaSearch(int side, int currentDepth, int alpha, int beta)
{
	if(currentDepth == maxDepth)
		return eval();

	if(wK == 0)
		return -10000;

	if(bK == 0)
		return 10000; 

    Moves * m = generateLegalMoves(side%2);

    if(currentDepth == 1)
    {
        //check if first move caused a repetition.
        for(int i = 0; i < 1000; i++)
        {
            if (wK == prevwK[i] &&
                bK == prevbK[i] &&
                wP == prevwP[i] &&
                bP == prevbP[i] &&
                wR == prevwR[i] &&
                bR == prevbR[i] &&
                wB == prevwB[i] &&
                bB == prevbB[i] &&
                wQ == prevwQ[i] &&
                bQ == prevbQ[i] &&
                wN == prevwN[i] &&
                bN == prevbN[i]
                )
            {
                return 0;
            }

            if (0 == prevwK[i] &&
                0 == prevbK[i] &&
                0 == prevwP[i] &&
                0 == prevbP[i] &&
                0 == prevwR[i] &&
                0 == prevbR[i] &&
                0 == prevwB[i] &&
                0 == prevbB[i] &&
                0 == prevwQ[i] &&
                0 == prevbQ[i] &&
                0 == prevwN[i] &&
                0 == prevbN[i]
                )
            {
                break;
            }
            
        }
    }

	
    if(m->numMoves == 0)
	{
		bestMove = 0;
        free(m);
		return eval();
	}

	if(currentDepth == 0)
	{
		bestMove = m->move[0];
		if(m->numMoves == 1)
        {
            free(m);
			return eval();
        }
	}

	if(side % 2 == 1)
	{
		for(int i = 0; i < m->numMoves; i++)
		{
			int start = Moves_getStart(m, i);
			int end = Moves_getEnd(m, i);
            int flags = Moves_getFlags(m, i);
			
			unsigned long long tempWP = wP;
			unsigned long long tempWQ = wQ;
			unsigned long long tempWK = wK;
			unsigned long long tempWR = wR;
			unsigned long long tempWN = wN;
			unsigned long long tempWB = wB;
			unsigned long long tempBP = bP;
            unsigned long long tempBQ = bQ;
            unsigned long long tempBK = bK;
            unsigned long long tempBR = bR;
            unsigned long long tempBN = bN;
            unsigned long long tempBB = bB;
            int wkingtemp = whiteKingside;
            int wqueentemp = whiteQueenside;
            int bkingtemp = blackKingside;
            int bqueentemp = blackQueenside;
            unsigned long long tempenpassant = enpassantsquare;

			makeMove(start, end, flags);
            if(currentDepth <= 1 && check(1))
            {                
                enpassantsquare = tempenpassant;
                whiteKingside = wkingtemp;
                whiteQueenside = wqueentemp;
                blackKingside = bkingtemp;
                blackQueenside = bqueentemp;
                wK = tempWK;
                wQ = tempWQ;
                wP = tempWP;
                wN = tempWN;
                wB = tempWB;
                wR = tempWR;
                bK = tempBK;
                bQ = tempBQ;
                bP = tempBP;
                bN = tempBN;
                bB = tempBB;
                bR = tempBR;
                continue;
            }
			int result = alphaBetaSearch(side + 1, currentDepth+1, alpha, beta);			

            enpassantsquare = tempenpassant;
            whiteKingside = wkingtemp;
            whiteQueenside = wqueentemp;
            blackKingside = bkingtemp;
            blackQueenside = bqueentemp;
			wK = tempWK;
			wQ = tempWQ;
			wP = tempWP;
			wN = tempWN;
			wB = tempWB;
			wR = tempWR;
			bK = tempBK;
            bQ = tempBQ;
            bP = tempBP;
            bN = tempBN;
            bB = tempBB;
            bR = tempBR;


			if(result > alpha)
			{
				alpha = result;
		
				if(currentDepth == 0)
					bestMove = m->move[i];

			}

			if(alpha >= beta)
			{
                free(m);
				return alpha;
			}
		}

        free(m);
		return alpha;
	}

	else
	{
		for(int i = 0; i < m->numMoves; i++)
		{
			int start = Moves_getStart(m, i);
			int end = Moves_getEnd(m, i);
            int flags = Moves_getFlags(m, i);
			
			unsigned long long tempWP = wP;
            unsigned long long tempWQ = wQ;
            unsigned long long tempWK = wK;
            unsigned long long tempWR = wR;
            unsigned long long tempWN = wN;
            unsigned long long tempWB = wB;
            unsigned long long tempBP = bP;
            unsigned long long tempBQ = bQ;
            unsigned long long tempBK = bK;
            unsigned long long tempBR = bR;
            unsigned long long tempBN = bN;
            unsigned long long tempBB = bB;
            int wkingtemp = whiteKingside;
            int wqueentemp = whiteQueenside;
            int bkingtemp = blackKingside;
            int bqueentemp = blackQueenside;
            unsigned long long tempenpassant = enpassantsquare;

            makeMove(start, end, flags);
            if(currentDepth <= 1 && check(2))
            {
                wK = tempWK;
                wQ = tempWQ;
                wP = tempWP;
                wN = tempWN;
                wB = tempWB;
                wR = tempWR;
                bK = tempBK;
                bQ = tempBQ;
                bP = tempBP;
                bN = tempBN;
                bB = tempBB;
                bR = tempBR;
                whiteKingside = wkingtemp;
                whiteQueenside = wqueentemp;
                blackKingside = bkingtemp;
                blackQueenside = bqueentemp;
                enpassantsquare = tempenpassant;
                continue;
            }
            int result = alphaBetaSearch(side + 1, currentDepth+1, alpha, beta);

            wK = tempWK;
            wQ = tempWQ;
            wP = tempWP;
            wN = tempWN;
            wB = tempWB;
            wR = tempWR;
            bK = tempBK;
            bQ = tempBQ;
            bP = tempBP;
            bN = tempBN;
            bB = tempBB;
			bR = tempBR;
            whiteKingside = wkingtemp;
            whiteQueenside = wqueentemp;
            blackKingside = bkingtemp;
            blackQueenside = bqueentemp;
            enpassantsquare = tempenpassant;

			if(result < beta)
			{
				beta = result;

				if(currentDepth == 0)
					bestMove = m->move[i];

			}
				
			if(beta <= alpha)
			{
                free(m);
				return beta;
			}
	
		}
	
        free(m);
		return beta;
	}
}
