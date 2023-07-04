#include "xfcntl.h"

#include <err.h>
#include <fcntl.h>

int xopen(const char *pathname, int flags, mode_t mode)
{
    int fd = open(pathname, flags, mode);
    if (fd == -1)
        err(1, "fail to open %s", pathname);
    return fd;
}
