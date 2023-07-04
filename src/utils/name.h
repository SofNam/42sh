#ifndef NAME_H
#define NAME_H

#include <stdbool.h>
#include <stddef.h>

/// @file name.h
/// @brief Definition of function to check validity of a given name.

/// @brief Check if given string is a valid char
/// according to Base Definitions volume of IEEE Std 1003.1-2001, Section 3.230
bool is_valid_char(const char c);

/// @brief Check if given string is a valid name
/// according to Base Definitions volume of IEEE Std 1003.1-2001, Section 3.230
bool is_valid_name(char *name, char *end);

#endif /* ! NAME_H */
