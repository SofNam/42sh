#ifndef XMALLOC_H
#define XMALLOC_H

#include <stddef.h>

/// @file xmalloc.h
/// @brief Safe implementations of functions (de)allocating dynamic memory.

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t nmemb, size_t size);

#endif /* ! XMALLOC_H */
