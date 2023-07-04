#ifndef XIO_H
#define XIO_H

#include <stdio.h>

/// @file xio.h
/// @brief Safe implementation of standard input/output library functions.

FILE *xfdopen(int fd, const char *mode);
FILE *xfopen(const char *filename, const char *mode);
void xfseek(FILE *stream, long offset, int whence);
long xftell(FILE *stream);
void xfclose(FILE *stream);
size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream);

#endif /* ! XIO_H */
