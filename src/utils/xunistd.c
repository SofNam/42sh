#include "xunistd.h"

#include <err.h>
#include <errno.h>
#include <unistd.h>

void xpipe(int *fds)
{
    if (pipe(fds) == -1)
        err(1, "fail to pipe.");
}

int xdup(int fildes)
{
    int res = dup(fildes);
    if (res == -1)
        err(1, "fail to dup file descriptor %d.", fildes);
    return res;
}

void xdup2(int fildes, int fildes2)
{
    if (dup2(fildes, fildes2) == -1)
        err(1, "fail to dup file descriptors %d to %d", fildes2, fildes);
}

pid_t xfork(void)
{
    int pid = fork();
    if (pid == -1)
        err(1, "failed to fork");
    return pid;
}

void xclose(int fd)
{
    if (close(fd) == -1 && errno != EBADF)
        err(1, "fail to close file descriptor");
}
