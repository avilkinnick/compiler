#ifndef HASH_TABLE_STRING_H
#define HASH_TABLE_STRING_H

#include "LinearAllocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct HashTableString
{
    LinearAllocator allocator;
    size_t hashes_offset;
    size_t lengths_offset;
    size_t busy_slots_offset;
    uint32_t size;
    uint32_t capacity;
} HashTableString;

void hash_table_string_zero(HashTableString* const table);

bool hash_table_string_create(
    HashTableString* const table,
    const uint32_t capacity,
    const size_t strings_buffer_size
);

void hash_table_string_destroy(HashTableString* const table);

#endif // HASH_TABLE_STRING_H
