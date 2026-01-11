#include "HashTableString.h"

#include "LinearAllocator.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
    const size_t strings_buffer_capacity
)
{
    assert(table != NULL);
    assert(table->offsets == NULL);
    assert(table->hashes == NULL);
    assert(table->lengths == NULL);
    assert(table->size == 0);
    assert(table->capacity == 0);
    assert(capacity > 0);
    assert(strings_buffer_capacity >= 2 * (size_t)capacity);

    return true;
}
