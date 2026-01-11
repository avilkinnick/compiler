#ifndef HASH_TABLE_STRING_H
#define HASH_TABLE_STRING_H

#include "LinearAllocator.h"

#include <stddef.h>
#include <stdint.h>

typedef struct HashTableString
{
    LinearAllocator allocator;
    size_t* offsets;
    uint32_t* hashes;
    uint16_t* lengths;
    uint32_t size;
    uint32_t capacity;
} HashTableString;

void hash_table_string_zero(HashTableString* const table);

#endif // HASH_TABLE_STRING_H
