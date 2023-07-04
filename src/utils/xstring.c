#include "xstring.h"

#include <err.h>
#include <string.h>

char *xstrndup(const char *s, size_t n)
{
    if (s == NULL || n == 0)
        return NULL;
    char *dup = strndup(s, n);
    if (dup == NULL)
        err(1, "strndup: Failed dup string");
    return dup;
}

char *xstrdup(const char *s)
{
    if (s == NULL)
        return NULL;
    char *dup = strdup(s);
    if (dup == NULL)
        err(1, "strndup: Failed dup string");
    return dup;
}

size_t xstrlen(const char *s)
{
    return s == NULL ? 0 : strlen(s);
}

size_t digit_count(int x)
{
    size_t digits = 1;
    while (x > 9)
    {
        digits++;
        x /= 10;
    }
    return digits;
}
