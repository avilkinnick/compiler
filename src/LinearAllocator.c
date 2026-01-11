#include "LinearAllocator.h"

#include "math_funcs.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void linear_allocator_zero(LinearAllocator* const allocator)
{
    assert(allocator != NULL);

    allocator->data = NULL;
    allocator->offset = 0;
    allocator->capacity = 0;
}

bool linear_allocator_create(
    LinearAllocator* const allocator,
    const size_t capacity
)
{
    assert(allocator != NULL);
    assert(allocator->data == NULL);
    assert(allocator->offset == 0);
    assert(allocator->capacity == 0);
    assert(capacity > 0);

    allocator->data = malloc(capacity);
    if (allocator->data == NULL)
    {
        fprintf(stderr, "Failed to create linear allocator "
            "with capacity %zu\n", capacity);

        return false;
    }

    allocator->offset = 0;
    allocator->capacity = capacity;

    return true;
}

void linear_allocator_destroy(LinearAllocator* const allocator)
{
    assert(allocator != NULL);
    assert(allocator->offset <= allocator->capacity);

    free(allocator->data);
    allocator->data = NULL;

    linear_allocator_zero(allocator);
}

void* linear_allocator_allocate(
    LinearAllocator* const allocator,
    const size_t size,
    const size_t alignment
)
{
    assert(allocator != NULL);
    assert(allocator->data != NULL);
    assert(allocator->offset <= allocator->capacity);
    assert(allocator->capacity > 0);
    assert(size > 0);
    assert(alignment <= allocator->capacity);
    assert(is_power_of_two(alignment));

    size_t aligned_offset;
    if (!ALIGN_UP(allocator->offset, alignment, &aligned_offset))
    {
        return NULL;
    }

    if ((aligned_offset > allocator->capacity)
        || (size > allocator->capacity - aligned_offset))
    {
        fprintf(stderr, "Linear allocator could not allocate memory\n"
            "offset: %zu; capacity: %zu; requested size: %zu;\n"
            "requested alignment: %zu; aligned offset: %zu\n",
            allocator->offset, allocator->capacity, size,
            alignment, aligned_offset);

        return NULL;
    }

    void* const ptr = (char*)allocator->data + aligned_offset;

    allocator->offset = aligned_offset + size;

    return ptr;
}
