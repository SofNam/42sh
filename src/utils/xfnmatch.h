#ifndef XFNMATCH_H
#define XFNMATCH_H

#include <stdbool.h>

/// @file xfnmatch.h
/// @brief Safe implementation of fnmatch.

bool xfnmatch(const char *pattern, const char *string, int flags);

#endif /* ! XFNMATCH_H */
