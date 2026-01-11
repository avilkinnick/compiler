#include "math_funcs.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

bool is_power_of_two(const size_t x)
{
    return x && !(x & (x - 1));
}

bool align_up(
    const size_t value,
    const char* const name,
    const size_t alignment,
    size_t* const aligned_value,
    const char* const file,
    const int line
)
{
    assert(name != NULL);
    assert(is_power_of_two(alignment));
    assert(aligned_value != NULL);
    assert(file != NULL);
    assert(line > 0);

    const size_t alignment_mask = alignment - 1;

    if (value > SIZE_MAX - alignment_mask)
    {
        fprintf(stderr, "%s:%d: Failed to align up %s\n"
            "(%s > SIZE_MAX - alignment_mask)\n"
            "%s: %zu; alignment: %zu; alignment_mask: %zu\n",
            file, line, name, name, name, value, alignment, alignment_mask);

        return false;
    }

    *aligned_value = (value + alignment_mask) & ~alignment_mask;

    return true;
}
