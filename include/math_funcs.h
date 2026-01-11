#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H

#include <stdbool.h>
#include <stddef.h>

#define ALIGN_UP(value, alignment, aligned_value)                            \
    align_up(value, #value, alignment, aligned_value, __FILE__, __LINE__)    \

bool is_power_of_two(const size_t x);

bool align_up(
    const size_t value,
    const char* const name,
    const size_t alignment,
    size_t* const aligned_value,
    const char* const file,
    const int line
);

#endif // MATH_FUNCS_H
