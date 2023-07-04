#ifndef XSTRING_H
#define XSTRING_H

#include <stddef.h>

/// @file xstring.h
/// @brief Safe implementations of string library functions.

char *xstrndup(const char *s, size_t n);
char *xstrdup(const char *s);
size_t xstrlen(const char *s);
size_t digit_count(int x);

#endif /* ! XSTRING_H */
