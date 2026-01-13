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

LinearAllocatorResult linear_allocator_create_arrays(
    LinearAllocator* const allocator,
    const int array_count,
    const size_t element_count,
    size_t* const sizes,
    const size_t* const alignments,
    const size_t additional_memory_size,
    size_t* const * const offsets
)
{
    assert(allocator != NULL);
    assert(allocator->data == NULL);
    assert(allocator->offset == 0);
    assert(allocator->capacity == 0);
    assert(array_count > 0);
    assert(element_count > 0);
    assert(sizes != NULL);
    assert(alignments != NULL);
    assert(offsets != NULL);

    size_t total_size = 0;

    for (int i = 0; i < array_count; ++i)
    {
        if (sizes[i] > SIZE_MAX / element_count)
        {
            fprintf(stderr, "Overflow\n"
                "(sizes[i] > SIZE_MAX / element_count)\n"
                "sizes[i]: %zu; element_count: %zu\n",
                sizes[i], element_count);

            return LINEAR_ALLOCATOR_ERROR_OVERFLOW;
        }

        sizes[i] *= element_count;

        if (!ALIGN_UP(sizes[i], alignments[i], &sizes[i]))
        {
            return LINEAR_ALLOCATOR_ERROR_OVERFLOW;
        }

        if (total_size > SIZE_MAX - sizes[i])
        {
            fprintf(stderr, "Overflow\n"
                "(total_size > SIZE_MAX - sizes[i])\n"
                "total_size: %zu; sizes[i]: %zu\n",
                total_size, sizes[i]);

            return LINEAR_ALLOCATOR_ERROR_OVERFLOW;
        }

        total_size += sizes[i];
    }

    if (additional_memory_size > SIZE_MAX - total_size)
    {
        fprintf(stderr, "Overflow\n"
            "(additional_memory_size > SIZE_MAX - total_size)n"
            "additional_memory_size: %zu; total_size: %zu\n",
            additional_memory_size, total_size);

        return LINEAR_ALLOCATOR_ERROR_OVERFLOW;
    }

    total_size += additional_memory_size;

    if (!linear_allocator_create(allocator, total_size))
    {
        return LINEAR_ALLOCATOR_ERROR_CREATE;
    }

    for (int i = 0; i < array_count; ++i)
    {
        if (offsets[i] != NULL)
        {
            *offsets[i] = allocator->offset;
        }

        if (!linear_allocator_allocate(allocator, sizes[i], alignments[i]))
        {
            linear_allocator_destroy(allocator);

            return LINEAR_ALLOCATOR_ERROR_ALLOCATE;
        }
    }

    return LINEAR_ALLOCATOR_SUCCESS;
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

    if ((aligned_offset > allocator->capacity) ||
        (size > allocator->capacity - aligned_offset))
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
