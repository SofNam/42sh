#include <err.h>
#include <stdlib.h>

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
        err(1, "malloc failed");
    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
        err(1, "realloc failed");
    return new_ptr;
}

void *xcalloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (ptr == NULL)
        err(1, "calloc failed");
    return ptr;
}
