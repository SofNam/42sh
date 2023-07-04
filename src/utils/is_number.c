#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

bool is_number(const char *string)
{
    size_t i = 0;
    if (string[i] == '-')
    {
        if (string[++i] == '\0')
            return false;
    }
    for (; string[i] != '\0'; i++)
    {
        if (!isdigit(string[i]))
            return false;
    }
    return true;
}
