#include "magicGeneration.h"

void generateRookMasks()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        for(int i = boardPosition+8; i <= 55; i+=8)
            rookMask[boardPosition] |= 1LL << i;
        for(int i = boardPosition-8; i >= 8; i-=8)
            rookMask[boardPosition] |= 1LL << i;

        unsigned long long right = 1LL << boardPosition;
        unsigned long long left = 1LL << boardPosition;

        for(int i = 0; i < 8; i++)
        {
            right = (right << 1) & notHFile & notAFile;
            rookMask[boardPosition] |= right;

            left = (left >> 1) & notAFile & notHFile;
            rookMask[boardPosition] |= left;
        }
    }

    for(int i = 0; i < 64; i++)
    {
        unsigned long long temp = rookMask[i];
        magicShiftRooks[i] = 64;
        bitsSetRook[i] = 0;        

        while(temp != 0)
        {
            magicShiftRooks[i]--;
            bitsSetRook[i]++;
            temp &= temp-1;
        }
    }
}

void generateBishopMasks()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        unsigned long long upperLeft = 1LL << boardPosition;
        unsigned long long upperRight = upperLeft;
        unsigned long long lowerLeft = upperLeft;
        unsigned long long lowerRight = upperLeft;

        unsigned long long mask = notRank1 & notRank8 &notAFile &notHFile;

        for(int i = 0; i < 8; i++)
        {
            upperLeft = (upperLeft << 7) & mask;
            bishopMask[boardPosition] |= upperLeft;
            upperRight = (upperRight << 9) & mask;
            bishopMask[boardPosition] |= upperRight;
            lowerLeft = (lowerLeft >> 9) & mask;
            bishopMask[boardPosition] |= lowerLeft;
            lowerRight = (lowerRight >> 7) & mask;
            bishopMask[boardPosition] |= lowerRight;
        }
    }

    for(int i = 0; i < 64; i++)
    {
        unsigned long long temp = bishopMask[i];
        magicShiftBishops[i] = 64;
        bitsSetBishop[i] = 0;

        while(temp != 0)
        {
            magicShiftBishops[i]--;
            bitsSetBishop[i]++;
            temp &= temp-1;
        }
    }
}

int* getBits(unsigned long long bitboard)
{
    int * temp = malloc(64 * sizeof(int));
    int current = 0;

    for(int i = 0; i < 64; i++)
    {
        if(bitboard % 2)
        {
            temp[current] = i;
            current++;
        }

        bitboard /= 2;
    }

    temp[current] = -1;

    return temp;
}

void generateRookBlockers()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        int * maskBitsSet = getBits(rookMask[boardPosition]);
        int variations = (int) (1LL << bitsSetRook[boardPosition]);

        rookBlockers[boardPosition] = calloc(variations, sizeof(unsigned long long));

        for(int i = 0; i < variations; i++)
        {
            int * indexBitsSet = getBits(i);            

            for(int j = 0; indexBitsSet[j] != -1; j++)
            {
                rookBlockers[boardPosition][i] |= (1LL << maskBitsSet[indexBitsSet[j]]);
            }

            free(indexBitsSet);
        }

        free(maskBitsSet);
    }
}

void generateBishopBlockers()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        int * maskBitsSet = getBits(bishopMask[boardPosition]);
        int variations = (int) (1LL << bitsSetBishop[boardPosition]);

        bishopBlockers[boardPosition] = calloc(variations, sizeof(unsigned long long));

        for(int i = 0; i < variations; i++)
        {
            int * indexBitsSet = getBits(i);

            for(int j = 0; indexBitsSet[j] != -1; j++)
            {
                bishopBlockers[boardPosition][i] |= (1LL << maskBitsSet[indexBitsSet[j]]);
            }

            free(indexBitsSet);
        }

        free(maskBitsSet);
    }
}

void generateRookAttacks()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        int variations = (int) (1LL << bitsSetRook[boardPosition]);
        rookAttacks[boardPosition] = calloc(variations, sizeof(unsigned long long));

        for(int i = 0; i < variations; i++)
        {
            unsigned long long right = 1LL << boardPosition;
            unsigned long long left = 1LL << boardPosition;
            unsigned long long up = 1LL << boardPosition;
            unsigned long long down = 1LL << boardPosition;
            unsigned long long mask = rookMask[boardPosition];

            for(int j = 0; j < 8; j++)
            {
                unsigned long long temp;

                temp = (right << 1) & mask & ~rookBlockers[boardPosition][i];
                right = (temp == 0) ? right : temp;
                
                temp = (left >> 1) & mask & ~rookBlockers[boardPosition][i];
                left = (temp == 0) ? left : temp;

                temp = (up << 8) & mask & ~rookBlockers[boardPosition][i];
                up = (temp == 0) ? up : temp;

                temp = (down >> 8) & mask & ~rookBlockers[boardPosition][i];
                down = (temp == 0) ? down : temp;
            }
            
            right = (right << 1) & mask;
            left = (left >> 1) & mask;
            up = (up << 8) & mask;
            down = (down >> 8) & mask;
            
            rookAttacks[boardPosition][i] |= right | left | up | down;
        /*

        int j;

        for (j=boardPosition+8; j<=55 && (rookBlockers[boardPosition][i] & (1LL << j)) == 0; j+=8);
        
        if (j>=0 && j<=63)
            rookAttacks[boardPosition][i] |= (1LL << j);
        
        for (j=boardPosition-8; j>=8 && (rookBlockers[boardPosition][i] & (1LL << j)) == 0; j-=8);

        if (j>=0 && j<=63) 
            rookAttacks[boardPosition][i] |= (1LL << j);
        
        for (j=boardPosition+1; j%8!=7 && j%8!=0 && (rookBlockers[boardPosition][i] & (1LL << j)) == 0; j++);
        
        if (j>=0 && j<=63) 
            rookAttacks[boardPosition][i] |= (1LL << j);
                    
        for (j=boardPosition-1; j%8!=7 && j%8!=0 && j>=0 && (rookBlockers[boardPosition][i] & (1LL << j)) == 0; j--);
                    
        if (j>=0 && j<=63) 
                rookAttacks[boardPosition][i] |= (1LL << j);

        */
        }
    }
}

void generateBishopAttacks()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        int variations = (int) (1LL << bitsSetBishop[boardPosition]);
        bishopAttacks[boardPosition] = calloc(variations, sizeof(unsigned long long));

        for(int i = 0; i < variations; i++)
        {
            unsigned long long right = 1LL << boardPosition;
            unsigned long long left = 1LL << boardPosition;
            unsigned long long up = 1LL << boardPosition;
            unsigned long long down = 1LL << boardPosition;
            unsigned long long mask = bishopMask[boardPosition];

            for(int j = 0; j < 8; j++)
            {
                unsigned long long temp;

                temp = (right << 7) & mask & ~bishopBlockers[boardPosition][i];
                right = (temp == 0) ? right : temp;

                temp = (left >> 7) & mask & ~bishopBlockers[boardPosition][i];
                left = (temp == 0) ? left : temp;

                temp = (up << 9) & mask & ~bishopBlockers[boardPosition][i];
                up = (temp == 0) ? up : temp;

                temp = (down >> 9) & mask & ~bishopBlockers[boardPosition][i];
                down = (temp == 0) ? down : temp;
            }

            right = (right << 7) & mask;
            left = (left >> 7) & mask;
            up = (up << 9) & mask;
            down = (down >> 9) & mask;

            bishopAttacks[boardPosition][i] |= right | left | up | down;
        }
    }
}

void generateRookMagic()
{
    int randomSrc = open("/dev/random", O_RDONLY);
    unsigned long long rand[3];

    FILE * file = fopen("rookMagic.txt", "w");

    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        unsigned long long variations = 1LL << bitsSetRook[boardPosition];
        unsigned long long * used = malloc(variations * sizeof(unsigned long long));

        int attempts = 0;
        int fail, i;
        unsigned long long magic;

        do
        {
            if(read(randomSrc, rand, 3 * sizeof(unsigned long long)) == -1)
            {
                perror("read failed"); 
                exit(EXIT_FAILURE);
            }

            magic = rand[0] & rand[1] & rand[2];
            attempts++;

            memset(used, 0, variations * sizeof(unsigned long long));
                    
            for(i = 0, fail = 0; i < variations && !fail; i++)
            {
                int index = (int)((rookBlockers[boardPosition][i] * magic) >> magicShiftRooks[boardPosition]);
                
                fail = (used[index] != 0 && used[index] != rookAttacks[boardPosition][i]);

                used[index] = rookAttacks[boardPosition][i];

            }
            
            if(i == variations - 1)
            {
                puts("Damn, so close");
            }

        }while(fail);
        
        printf("Magic number = %lld\n", magic);        

        fprintf(file, "rookMagic[%d] = %lld;\n", boardPosition, magic);

        free(used);
        rookMagic[boardPosition] = magic;
    }

    fclose(file);
    close(randomSrc);
}

void generateBishopMagic()
{
    int randomSrc = open("/dev/random", O_RDONLY);
    unsigned long long rand[3];

    FILE * file = fopen("bishopMagic.txt", "w");

    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        unsigned long long variations = 1LL << bitsSetBishop[boardPosition];
        unsigned long long * used = malloc(variations * sizeof(unsigned long long));

        int attempts = 0;
        int fail, i;
        unsigned long long magic;

        do
        {
            if(read(randomSrc, rand, 3 * sizeof(unsigned long long)) == -1)
            {
                perror("read failed");
                exit(EXIT_FAILURE);
            }

            magic = rand[0] & rand[1] & rand[2];
            attempts++;

            memset(used, 0, variations * sizeof(unsigned long long));

            for(i = 0, fail = 0; i < variations && !fail; i++)
            {
                int index = (int)((bishopBlockers[boardPosition][i] * magic) >> magicShiftBishops[boardPosition]);

                fail = (used[index] != 0 && used[index] != bishopAttacks[boardPosition][i]);

                used[index] = bishopAttacks[boardPosition][i];
            }

        }while(fail);

        printf("bishopMagic[%d] = %lld;\n", boardPosition, magic);
        fprintf(file, "bishopMagic[%d] = %lld;\n", boardPosition, magic);

        bishopMagic[boardPosition] = magic;

        free(used);
    }
    
    fclose(file);
    close(randomSrc);
}

void generateRookMoveArray()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        int variations = (1LL << bitsSetRook[boardPosition]);
        rookMoves[boardPosition] = calloc(variations, sizeof(unsigned long long));

        for(int i = 0; i < variations; i++)
        {
            unsigned long long right = 1LL << boardPosition;
            unsigned long long left = 1LL << boardPosition;
            unsigned long long up = 1LL << boardPosition;
            unsigned long long down = 1LL << boardPosition;
            unsigned long long mask = rookMask[boardPosition];

            int index = (int) ((rookBlockers[boardPosition][i] * rookMagic[boardPosition]) >> magicShiftRooks[boardPosition]);

            for(int j = 0; j < 8; j++)
            {
                unsigned long long temp;

                temp = (right << 1) & mask & ~rookBlockers[boardPosition][i];
                right = (temp == 0) ? right : temp;

                temp = (left >> 1) & mask & ~rookBlockers[boardPosition][i];
                left = (temp == 0) ? left : temp;

                temp = (up << 8) & mask & ~rookBlockers[boardPosition][i];
                up = (temp == 0) ? up : temp;

                temp = (down >> 8) & mask & ~rookBlockers[boardPosition][i];
                down = (temp == 0) ? down : temp;

                rookMoves[boardPosition][index] |= right | left | up | down;
            }

            right = (right << 1) & notAFile;
            left = (left >> 1) & notHFile;
            up = (up << 8);
            down = (down >> 8);

            rookMoves[boardPosition][index] |= right | left | up | down;
        }
    }    
}

void generateBishopMoveArray()
{
    for(int boardPosition = 0; boardPosition < 64; boardPosition++)
    {
        int variations = (1LL << bitsSetBishop[boardPosition]);
        bishopMoves[boardPosition] = calloc(variations, sizeof(unsigned long long));

        for(int i = 0; i < variations; i++)
        {
            unsigned long long right = 1LL << boardPosition;
            unsigned long long left = 1LL << boardPosition;
            unsigned long long up = 1LL << boardPosition;
            unsigned long long down = 1LL << boardPosition;
            unsigned long long mask = bishopMask[boardPosition];

            int index = (int) ((bishopBlockers[boardPosition][i] * bishopMagic[boardPosition]) >> magicShiftBishops[boardPosition]);

            for(int j = 0; j < 8; j++)
            {
                unsigned long long temp;

                temp = (right << 7) & mask & ~bishopBlockers[boardPosition][i];
                right = (temp == 0) ? right : temp;

                temp = (left >> 7) & mask & ~bishopBlockers[boardPosition][i];
                left = (temp == 0) ? left : temp;

                temp = (up << 9) & mask & ~bishopBlockers[boardPosition][i];
                up = (temp == 0) ? up : temp;

                temp = (down >> 9) & mask & ~bishopBlockers[boardPosition][i];
                down = (temp == 0) ? down : temp;

                bishopMoves[boardPosition][index] |= right | left | up | down;
            }

            right = (right << 7) & notHFile;
            left = (left >> 7) & notAFile;
            up = (up << 9) & notAFile;
            down = (down >> 9) & notHFile;

            bishopMoves[boardPosition][index] |= right | left | up | down;
        }
    }
}

