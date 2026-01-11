#include "HashTableString.h"

#include "LinearAllocator.h"

#include <assert.h>
#include <stddef.h>

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
