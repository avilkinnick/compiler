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

void linear_allocator_zero(LinearAllocator* const allocator);

bool linear_allocator_create(
    LinearAllocator* const allocator,
    const size_t capacity
);

void linear_allocator_destroy(LinearAllocator* const allocator);

void* linear_allocator_allocate(
    LinearAllocator* const allocator,
    const size_t size,
    const size_t alignment
);

#endif // LINEAR_ALLOCATOR_H
