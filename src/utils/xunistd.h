#ifndef XUNISTD_H
#define XUNISTD_H

#include <sys/types.h>

/// @file xunistd.h
/// @brief Safe implementations of unistd library functions.

void xpipe(int *fds);
int xdup(int fildes);
void xdup2(int fildes, int fildes2);
pid_t xfork(void);
void xclose(int fd);
void xunlink(const char *path);

#endif /* ! XUNISTD_H */
