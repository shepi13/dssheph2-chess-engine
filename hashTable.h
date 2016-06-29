//A simple non-resizing hashtable that can be set to a specified size
//of memory
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "board.h"

struct elementStruct
{
    int eval;
    int depth;
    unsigned long long key;
};

typedef struct elementStruct element;

struct hashTableStruct
{
    element* table;
    int reqDepth;
    unsigned long long size;
};

typedef struct hashTableStruct hashTable;

void HashTable_create(hashTable* self, unsigned long long bytes);
void HashTable_destroy(hashTable* self);
int HashTable_get(hashTable* self, unsigned long long key);
void HashTable_put(hashTable* self, unsigned long long key, int value); 

#endif
