//File containing main function and actual chess game. Named two player because
//originally didn't call the engine ever in this file.
//TODO:
//add a feature where you can disable/enable the engine with a certain input
//clean up the terrible/inefficient code that exists in this file.
#include "board.h"
#include "eval.h"
#include "moveGeneration.h"

#define freeWKingside  !(Pieces & 0x0000000000000060)
#define freeWQueenside !(Pieces & 0x000000000000000e)
#define freeBKingside  !(Pieces & 0x6000000000000000)
#define freeBQueenside !(Pieces & 0xe000000000000000)

int getStart(char* input)
{
    unsigned int x = input[0]-97;
    unsigned int y = input[1]-49;

    return y*8+x;
}

int getEnd(char* input)
{
    int x = input[2]-97;
    int y = input[3]-49;

    return y*8+x;
}

int getFlags(char* input)
{
    if(!input[4])
        return 0;
    else if(input[4] == 'Q' || input[4] == 'q')
        return 1;
    else if(input[4] == 'N' || input[4] == 'n')
        return 2;
    else if(input[4] == 'R' || input[4] == 'r')
        return 3;
    else if(input[4] == 'B' || input[4] == 'b')
        return 4;
}


int main(int argc, char** argv)
{
    initializeBoard();
    char * inputMove = NULL;
    size_t n = 0;
    ssize_t length;

    int start;
    int end;

    int move = 1;
    int legal;

    do
    {
        printBoard();

        int evaluation = alphaBetaSearch(move%2, 0, -10000, 10000);
        printf("\nEval: %d\n", evaluation);
		printf("start: %lld\n", (bestMove >> 6) & 0x3f);
		printf("end: %lld\n", bestMove & 0x3f);

        legal = 0;
        length = getline(&inputMove, &n, stdin);

        inputMove[length-1] = 0;

        if(inputMove[0] == 'r')
        {
            destroyBoard();
            initializeBoard();
            move = 1;
            continue;
        }

        if(inputMove[0] == 'q')
        {
            break;
        }

        int start = getStart(inputMove);
        int end = getEnd(inputMove);
        int flags = getFlags(inputMove);
        Moves * moves = generateLegalMoves(move%2);
        
        for(int i = 0; i < moves->numMoves; i++)
        {
            if(start == Moves_getStart(moves, i) && end == Moves_getEnd(moves, i) && 
               flags == Moves_getFlags(moves, i))
            {
                legal = 1;
                break;
            }
        }

        if(!strcmp(inputMove, "0-0") && move % 2 && whiteKingside && freeWKingside)
        {
            makeMove(4, 6, 0);
            makeMove(7, 5, 0);
            move--;
            continue;
        }
        else if(!strcmp(inputMove, "0-0-0") && move % 2 && whiteQueenside && freeWQueenside)
        {
            makeMove(4, 2, 0);
            makeMove(0, 3, 0);
            move--;
            continue;
        }
        if(!strcmp(inputMove, "0-0") && move % 2 == 0 && blackKingside && freeBKingside)
        {
            makeMove(60, 62, 0);
            makeMove(63, 61, 0);
            move--;
            continue;
        }
        else if(!strcmp(inputMove, "0-0-0") && move % 2 == 0 && blackQueenside && freeBQueenside)
        {
            makeMove(60, 58, 0);
            makeMove(56, 59, 0);
            move--;
            continue;
        }

        if(legal)
        {
            makeMove(start, end, flags);
            move++;
        }
        else
        {
            printf("Moving from square %d to square %d is illegal, plz enter a legalMove\n", start, end);
            if(getline(&inputMove, &n, stdin) == -1);
            {
                perror("Getline failed");
            }
        }
    } while(inputMove != "q");    

    free(inputMove);
    destroyBoard();
       
    return 0;
}

