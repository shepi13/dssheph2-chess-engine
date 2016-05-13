#include "board.h"
#include "eval.h"

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

    for(int i = 0; i < 8; i++)
    {
        makeMove(i+8, i+24);
    }

    printBoard();

    do
    {
        printBoard();


        int evaluation = alphaBetaSearch(move%2, 0, -10000, 10000);
        printf("\nEval: %d\n", evaluation);
		printf("start: %d\n", (bestMove >> 6) & 0x3f);
		printf("end: %d\n", bestMove & 0x3f);


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
        Moves * moves = generateLegalMoves(move%2);
        
        for(int i = 0; i < moves->numMoves; i++)
        {
            if(start == Moves_getStart(moves, i) && end == Moves_getEnd(moves, i))
            {
                legal = 1;
                break;
            }
        }

        if(!strcmp(inputMove, "0-0") && move % 2 && whiteKingside && freeWKingside)
        {
            makeMove(4, 6);
            makeMove(7, 5);
            move--;
            continue;
        }
        else if(!strcmp(inputMove, "0-0-0") && move % 2 && whiteQueenside && freeWQueenside)
        {
            makeMove(4, 2);
            makeMove(0, 3);
            move--;
            continue;
        }
        if(!strcmp(inputMove, "0-0") && move % 2 == 0 && blackKingside && freeBKingside)
        {
            makeMove(60, 62);
            makeMove(63, 61);
            move--;
            continue;
        }
        else if(!strcmp(inputMove, "0-0-0") && move % 2 == 0 && blackQueenside && freeBQueenside)
        {
            makeMove(60, 58);
            makeMove(56, 59);
            move--;
            continue;
        }

        if(legal)
        {
            makeMove(start, end);
            move++;
        }
        else
        {
            printf("Moving from square %d to square %d is illegal, plz enter a legalMove\n", start, end);
            getline(&inputMove, &n, stdin);
        }
    } while(inputMove != "q");    

    free(inputMove);
    destroyBoard();
       
    return 0;
}

