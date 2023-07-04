#include <stddef.h>

#define PRIME 31

static size_t my_pow(size_t base, size_t power)
{
    size_t result = 1;
    while (1)
    {
        if (power & 1)
            result *= base;
        power >>= 1;
        if (power == 0)
            break;
        base *= base;
    }
    return result;
}

size_t hash(char *key, const size_t len)
{
    size_t hash = 0;
    size_t inter_value;
    for (size_t i = 0; i < len; i++)
    {
        inter_value = key[i] * my_pow(PRIME, len - i);
        hash += inter_value;
    }
    return hash;
}
