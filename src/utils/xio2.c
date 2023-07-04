#include <err.h>
#include <stdio.h>

FILE *xfdopen(int fd, const char *mode)
{
    FILE *file = fdopen(fd, mode);
    if (file == NULL)
        err(1, NULL);
    return file;
}
