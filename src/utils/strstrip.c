#include <string.h>
#include <xmalloc.h>

#include "xstring.h"

char *strstrip(char *str, char c)
{
    char *stripped = xcalloc(xstrlen(str) + 1, sizeof(char));
    size_t i = 0;
    for (; *str != '\0'; str++)
    {
        if (*str != c)
        {
            stripped[i++] = *str;
        }
    }
    stripped[i] = '\0';
    return stripped;
}
