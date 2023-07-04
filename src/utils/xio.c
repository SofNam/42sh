#include <err.h>
#include <stdio.h>

FILE *xfopen(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
        err(1, NULL);
    return file;
}

void xfseek(FILE *stream, long offset, int whence)
{
    int return_value = fseek(stream, offset, whence);
    if (return_value == -1)
        err(1, NULL);
}

long xftell(FILE *stream)
{
    long offset = ftell(stream);
    if (offset == -1)
        err(1, NULL);
    return offset;
}

void xfclose(FILE *stream)
{
    int return_value = fclose(stream);
    if (return_value == EOF)
        err(1, NULL);
}

size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t items_read = fread(ptr, size, nmemb, stream);
    if (items_read == 0 && ferror(stream) != 0)
        err(1, NULL);
    return items_read;
}
