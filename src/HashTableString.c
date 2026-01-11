#include "HashTableString.h"

#include "LinearAllocator.h"
#include "math_funcs.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void hash_table_string_zero(HashTableString* const table)
{
    assert(table != NULL);

    linear_allocator_zero(&table->allocator);

    table->offsets = NULL;
    table->hashes = NULL;
    table->lengths = NULL;
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
    assert(table->offsets == NULL);
    assert(table->hashes == NULL);
    assert(table->lengths == NULL);
    assert(table->size == 0);
    assert(table->capacity == 0);
    assert(capacity > 0);
    assert(strings_buffer_size >= 2 * (size_t)capacity);

    size_t offsets_size = capacity * sizeof(size_t);
    const size_t offsets_alignment = sizeof(uint32_t);
    if (!ALIGN_UP(offsets_size, offsets_alignment, &offsets_size))
    {
        return false;
    }

    size_t hashes_size = capacity * sizeof(uint32_t);
    const size_t hashes_alignment = sizeof(uint16_t);
    if (!ALIGN_UP(hashes_size, hashes_alignment, &hashes_size))
    {
        return false;
    }

    size_t lengths_size = capacity * sizeof(uint16_t);
    const size_t lengths_alignment = sizeof(char);
    if (!ALIGN_UP(lengths_size, lengths_alignment, &lengths_size))
    {
        return false;
    }

    size_t total_size = offsets_size + hashes_size + lengths_size;
    if (strings_buffer_size > SIZE_MAX - total_size)
    {
        fputs("Failed to create hash table string\n"
            "Too big strings buffer size\n", stderr);

        return false;
    }

    total_size += strings_buffer_size;

    if (!linear_allocator_create(&table->allocator, total_size))
    {
        return false;
    }

    table->offsets = linear_allocator_allocate(&table->allocator,
        offsets_size, offsets_alignment);

    if (table->offsets == NULL)
    {
        hash_table_string_destroy(table);

        return false;
    }

    table->hashes = linear_allocator_allocate(&table->allocator,
        hashes_size, hashes_alignment);

    if (table->hashes == NULL)
    {
        hash_table_string_destroy(table);

        return false;
    }

    table->lengths = linear_allocator_allocate(&table->allocator,
        lengths_size, lengths_alignment);

    if (table->lengths == NULL)
    {
        hash_table_string_destroy(table);

        return false;
    }

    table->size = 0;
    table->capacity = capacity;

    return true;
}

void hash_table_string_destroy(HashTableString* const table)
{
    assert(table != NULL);
    assert(table->size <= table->capacity);

    linear_allocator_destroy(&table->allocator);

    hash_table_string_zero(table);
}
