#include "board.h"

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

void setRookMagic()
{
    rookMagic[0] = -9187343066963213552;
    rookMagic[1] = 198175976864653312;
    rookMagic[2] = 108121577579610152;
    rookMagic[3] = -9151305508679843584;
    rookMagic[4] = 4647719215649259522;
    rookMagic[5] = -9079239222199581695;
    rookMagic[6] = 6196957489612062808;
    rookMagic[7] = -9151305362660261632;
    rookMagic[8] = 2392537843105920;
    rookMagic[9] = 72198606406287488;
    rookMagic[10] = -9223090149557006080;
    rookMagic[11] = 1407443603562752;
    rookMagic[12] = -9150751449911652332;
    rookMagic[13] = -9151173154968698368;
    rookMagic[14] = 37436725955526660;
    rookMagic[15] = 577305180599617792;
    rookMagic[16] = 36029071901138961;
    rookMagic[17] = 148619063118020608;
    rookMagic[18] = 1408475766726657;
    rookMagic[19] = 846959304980041728;
    rookMagic[20] = -8070431840466352128;
    rookMagic[21] = 576743326926144512;
    rookMagic[22] = 2452914235408124160;
    rookMagic[23] = 565149116400001;
    rookMagic[24] = 70370891825312;
    rookMagic[25] = 4505063137484800;
    rookMagic[26] = 600403996684845312;
    rookMagic[27] = 19782415355479168;
    rookMagic[28] = 13515199084496896;
    rookMagic[29] = 4614714075602223232;
    rookMagic[30] = 18577425772839188;
    rookMagic[31] = 422214613092608;
    rookMagic[32] = 18084769409537184;
    rookMagic[33] = 2324009415381884928;
    rookMagic[34] = 18172730406871041;
    rookMagic[35] = 74450165724876800;
    rookMagic[36] = 1731636256825148416;
    rookMagic[37] = 577727398296962048;
    rookMagic[38] = 571814833078789;
    rookMagic[39] = 36029363988201537;
    rookMagic[40] = 382955674780860416;
    rookMagic[41] = 4573970535874594;
    rookMagic[42] = 288283153246748800;
    rookMagic[43] = 70473199255584;
    rookMagic[44] = 144396697430196228;
    rookMagic[45] = 5189835689399812104;
    rookMagic[46] = 5190427162410483720;
    rookMagic[47] = -4611683542361833460;
    rookMagic[48] = 35459325495424;
    rookMagic[49] = 72339827080560896;
    rookMagic[50] = 598134594373760;
    rookMagic[51] = 2306125618599690496;
    rookMagic[52] = 1297041988377903360;
    rookMagic[53] = 18018796589580416;
    rookMagic[54] = 1152922606266089600;
    rookMagic[55] = 1116762833408;
    rookMagic[56] = 72621920622805010;
    rookMagic[57] = 288249068394790913;
    rookMagic[58] = 6926537326947470019;
    rookMagic[59] = 144203153303143433;
    rookMagic[60] = 288520648615067651;
    rookMagic[61] = 2594354897453318657;
    rookMagic[62] = 36207124196425860;
    rookMagic[63] = 144155921566695778;
}

void setBishopMagic()
{
    bishopMagic[0] = 297246577800546304;
    bishopMagic[1] = 967574829400064;
    bishopMagic[2] = 58551264088166656;
    bishopMagic[3] = 2258397957310548;
    bishopMagic[4] = 73483385741250696;
    bishopMagic[5] = 1171221913654265860;
    bishopMagic[6] = 284842511302672;
    bishopMagic[7] = 706453423849478;
    bishopMagic[8] = 36046409086312473;
    bishopMagic[9] = 36057685640151041;
    bishopMagic[10] = 9574552657068040;
    bishopMagic[11] = 4450148221766;
    bishopMagic[12] = 6940684762403078146;
    bishopMagic[13] = 6335532567104550;
    bishopMagic[14] = 216459858030231552;
    bishopMagic[15] = 6971713106704746496;
    bishopMagic[16] = 18016666306876416;
    bishopMagic[17] = 308497691440317504;
    bishopMagic[18] = -4535106644085800704;
    bishopMagic[19] = 1044985231280455680;
    bishopMagic[20] = 595042516010270848;
    bishopMagic[21] = 865254357598281984;
    bishopMagic[22] = 140807424446528;
    bishopMagic[23] = 2286997140476928;
    bishopMagic[24] = 74318194307764224;
    bishopMagic[25] = 144686942747363330;
    bishopMagic[26] = 4617949105193504;
    bishopMagic[27] = 9042933785329666;
    bishopMagic[28] = 1288627636150282;
    bishopMagic[29] = 6937232825969016960;
    bishopMagic[30] = 37507642327044096;
    bishopMagic[31] = 577595998078542852;
    bishopMagic[32] = 1173331465158002689;
    bishopMagic[33] = 18579693784535088;
    bishopMagic[34] = 1371381889382222434;
    bishopMagic[35] = 288243574604040448;
    bishopMagic[36] = -9222033930129244095;
    bishopMagic[37] = 2595204379116079104;
    bishopMagic[38] = 4617316626097504384;
    bishopMagic[39] = -8635651733512454072;
    bishopMagic[40] = 73399411077548032;
    bishopMagic[41] = 41677006003245060;
    bishopMagic[42] = 4666947473510515714;
    bishopMagic[43] = 38285278481227908;
    bishopMagic[44] = -9218657328813832927;
    bishopMagic[45] = 45603898341231106;
    bishopMagic[46] = 218433466092487685;
    bishopMagic[47] = 149538966884912;
    bishopMagic[48] = 1116960325632;
    bishopMagic[49] = 5634483912982534;
    bishopMagic[50] = 6926694780009713664;
    bishopMagic[51] = 1152922126337048577;
    bishopMagic[52] = 2533932474105888;
    bishopMagic[53] = 3751692141103481120;
    bishopMagic[54] = 4900079264101302272;
    bishopMagic[55] = -9223358808346943232;
    bishopMagic[56] = 7766644103232;
    bishopMagic[57] = 1152930341669901056;
    bishopMagic[58] = 13933037167347712;
    bishopMagic[59] = -9132727197131732992;
    bishopMagic[60] = 4756087080199848960;
    bishopMagic[61] = 2254093863356674;
    bishopMagic[62] = -9205356470095838448;
    bishopMagic[63] = 36170913208732170;
}

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
            read(randomSrc, rand, 3 * sizeof(unsigned long long)); 

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
            read(randomSrc, rand, 3 * sizeof(unsigned long long));

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
    if(piece == &bR && start == 56)
        blackQueenside = 0;
    if(piece == &bR && start == 63)
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
