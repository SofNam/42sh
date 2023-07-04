#include "name.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

bool is_valid_char(const char c)
{
    if (isalnum(c) || c == '_')
        return true;
    return false;
}

bool is_valid_name(char *name, char *end)
{
    if (isdigit(*name))
        return false;
    for (; name < end; name++)
    {
        if (!is_valid_char(*name))
            return false;
    }
    return true;
}
