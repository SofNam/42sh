#ifndef D_STRING_H
#define D_STRING_H

#include <stddef.h>

/// @def STRING_SIZE
/// By default the capacity of a string is set to 4.
#define STRING_SIZE 4

/// @file dstring.h
/// @brief Contains definition of string and related methods.

/// @struct string
/// Dynamic implementation of a string.
/// @var string::size
/// Number of characters held in the string.
/// @var string::capacity
/// Number of characters that can be held in the string.
/// @var string::str
/// Array of characters of size string::capacity.
struct string
{
    size_t size;
    size_t capacity;
    char *str;
};

/// Create an empty string.
struct string *string_create(void);

/// Double the capacity of the given string.
void string_resize(struct string *string);

/// Add a string to the string, resizing if necessary.
void string_add(struct string *string, char *str);

/// Add a characters to the string, resizing if necessary.
void string_add_char(struct string *string, char c);

/// @brief
/// Deallocate the space allocated to the given string,
/// freeing the held strings as well.
void string_destroy(struct string *string);

#endif /* ! D_STRING_H */
