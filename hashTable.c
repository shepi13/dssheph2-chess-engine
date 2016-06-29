#include "hashTable.h"

void hashTable_create(hashTable* self, unsigned long long bytes)
{
    self->size = bytes/sizeof(element);
    self->table = calloc(self->size, sizeof(element));
}

void hashTable_destroy(hashTable* self)
{
    free(self->table);
}

int hashTable_get(hashTable* self, unsigned long long key)
{
    unsigned long long idx = key % self->size;
    
    if(self->table[idx].key != key)
        return -1;

    return self->table[idx].eval;
}

void hashTable_put(hashTable* self, unsigned long long key, int value)
{
    unsigned long long idx = key % self->size;

    self->table[idx].key = key;
    self->table[idx].eval = value;
}
