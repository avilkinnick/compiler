#include "HashTableString.h"

#include "LinearAllocator.h"
#include "math_funcs.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum
{
    OFFSETS,
    HASHES,
    LENGTHS,
    BUSY_SLOTS
};

void hash_table_string_zero(HashTableString* const table)
{
    assert(table != NULL);

    linear_allocator_zero(&table->allocator);

    table->hashes_offset = 0;
    table->lengths_offset = 0;
    table->busy_slots_offset = 0;
    table->size = 0;
    table->capacity = 0;
}

bool hash_table_string_create(
    HashTableString* const table,
    const uint32_t capacity,
    const size_t strings_buffer_size
)
{
    assert(table != NULL);
    assert(table->hashes_offset == 0);
    assert(table->lengths_offset == 0);
    assert(table->busy_slots_offset == 0);
    assert(table->size == 0);
    assert(table->capacity == 0);
    assert(capacity > 0);
    assert(strings_buffer_size >= 2 * (size_t)capacity);

    size_t sizes[] = {
        [OFFSETS] = sizeof(size_t),
        [HASHES] = sizeof(uint32_t),
        [LENGTHS] = sizeof(uint16_t),
        [BUSY_SLOTS] = sizeof(uint32_t)
    };

    const size_t alignments[] = {
        [OFFSETS] = sizeof(uint32_t),
        [HASHES] = sizeof(uint16_t),
        [LENGTHS] = sizeof(uint32_t),
        [BUSY_SLOTS] = sizeof(char)
    };

    size_t* const offsets[] = {
        [OFFSETS] = NULL,
        [HASHES] = &table->hashes_offset,
        [LENGTHS] = &table->lengths_offset,
        [BUSY_SLOTS] = &table->busy_slots_offset
    };

    if (linear_allocator_create_arrays(&table->allocator, 4, capacity, sizes,
        alignments, strings_buffer_size, offsets) != LINEAR_ALLOCATOR_SUCCESS)
    {
        hash_table_string_destroy(table);

        return false;
    }

    memset(table->allocator.data, 0, table->hashes_offset);

    table->size = 0;
    table->capacity = capacity;

    return true;
}

void hash_table_string_destroy(HashTableString* const table)
{
    assert(table != NULL);
    assert(table->lengths_offset >= table->hashes_offset);
    assert(table->busy_slots_offset >= table->lengths_offset);
    assert(table->size <= table->capacity);

    linear_allocator_destroy(&table->allocator);

    hash_table_string_zero(table);
}
