#include <err.h>
#include <fnmatch.h>
#include <stdbool.h>

bool xfnmatch(const char *pattern, const char *string, int flags)
{
    int res = fnmatch(pattern, string, flags);
    if (res == 0)
        return true;
    if (res == FNM_NOMATCH)
        return false;
    err(1, "fail to fnmatch %s with %s", string, pattern);
}
