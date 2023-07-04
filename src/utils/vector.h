#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

/// @def VECTOR_SIZE
/// By default the capacity of a vector is set to 4.
#define VECTOR_SIZE 4

/// @file vector.h
/// @brief Contains definition of vector and related methods.

/// @struct vector
/// Dynamic implementation of a vector of strings.
/// @var vector::size
/// Number of strings held in the vector.
/// @var vector::capacity
/// Number of strings that can be held in the vector.
/// @var vector::strings
/// Array of strings of size vector::capacity.
struct vector
{
    size_t size;
    size_t capacity;
    void **data;
};

/// Create an empty vector.
struct vector *vector_create(void);

/// Double the capacity of the given vector.
void vector_resize(struct vector *vector);

/// Add a string to the vector, resizing if necessary.
void vector_add(struct vector *vector, void *datum);

/// @brief
/// Deallocate the space allocated to the given vector,
/// freeing the held strings as well.
void vector_destroy(struct vector *vector);

#endif /* ! VECTOR_H */
