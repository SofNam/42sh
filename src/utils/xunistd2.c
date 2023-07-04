#include <err.h>
#include <unistd.h>

void xunlink(const char *path)
{
    if (unlink(path) == -1)
        err(1, "fail to unlink");
}
