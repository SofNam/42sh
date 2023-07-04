#ifndef XFCNTL_H
#define XFCNTL_H

#include <fcntl.h>

/// @file xfcntl.h
/// @brief Safe implementation of fcntl

int xopen(const char *pathname, int flags, mode_t mode);

#endif /* ! XFCNTL_H */
