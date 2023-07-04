#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include <stddef.h>

/// @def VECTOR_SIZE
/// By default the capacity of a string_vector is set to 4.
#define VECTOR_SIZE 4

/// @file string_vector.h
/// @brief Contains definition of string_vector and related methods.

/// @struct string_vector
/// Dynamic implementation of a vector of strings.
/// @var string_vector::size
/// Number of strings held in the string_vector.
/// @var string_vector::capacity
/// Number of strings that can be held in the string_vector.
/// @var string_vector::strings
/// Array of strings of size string_vector::capacity.
struct string_vector
{
    size_t size;
    size_t capacity;
    char **strings;
};

/// Create an empty string_vector.
struct string_vector *string_vector_create(void);
/// Double the capacity of the given string_vector.
void string_vector_resize(struct string_vector *string_vector);
/// Add a string to the string_vector, resizing if necessary.
void string_vector_add(struct string_vector *string_vector, char *string);
/// Remove the last string of the string_vector
void string_vector_remove(struct string_vector *string_vector);
/// @brief
/// Deallocate the space allocated to the given string_vector,
/// freeing the held strings as well.
void string_vector_destroy(struct string_vector *string_vector);

#endif /* ! STRING_VECTOR_H */
