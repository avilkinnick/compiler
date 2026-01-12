#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct LinearAllocator
{
    void* data;
    size_t offset;
    size_t capacity;
} LinearAllocator;

typedef enum LinearAllocatorResult
{
    LINEAR_ALLOCATOR_SUCCESS,
    LINEAR_ALLOCATOR_ERROR_OVERFLOW,
    LINEAR_ALLOCATOR_ERROR_CREATE,
    LINEAR_ALLOCATOR_ERROR_ALLOCATE
} LinearAllocatorResult;

void linear_allocator_zero(LinearAllocator* const allocator);

bool linear_allocator_create(
    LinearAllocator* const allocator,
    const size_t capacity
);

LinearAllocatorResult linear_allocator_create_arrays(
    LinearAllocator* const allocator,
    const int array_count,
    const size_t element_count,
    size_t* const sizes,
    const size_t* const alignments,
    const size_t additional_memory_size,
    size_t* const * const offsets
);

void linear_allocator_destroy(LinearAllocator* const allocator);

void* linear_allocator_allocate(
    LinearAllocator* const allocator,
    const size_t size,
    const size_t alignment
);

#endif // LINEAR_ALLOCATOR_H
