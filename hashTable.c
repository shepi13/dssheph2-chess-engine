#include "hashTable.h"

void hashTable_create(hashTable* self, unsigned long long bytes, int reqDepth)
{
    self->size = bytes/sizeof(element);
    self->table = calloc(self->size, sizeof(element));
    self->reqDepth = reqDepth;
}

void hashTable_destroy(hashTable* self)
{
    free(self->table);
}

int hashTable_get(hashTable* self, unsigned long long key)
{
    unsigned long long idx = key % self->size;
    
    if(self->table[idx].key != key && self->table[idx].depth > self->reqDepth)
        return -1;

    return self->table[idx].eval;
}

void hashTable_put(hashTable* self, unsigned long long key, int eval, int depth)
{
    unsigned long long idx = key % self->size;

    self->table[idx].key = key;
    self->table[idx].eval = eval;
    self->table[idx].depth = depth;
}
