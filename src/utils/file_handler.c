#include <stdlib.h>

#include "xio.h"
#include "xmalloc.h"

static size_t get_file_size(FILE *file)
{
    xfseek(file, 0, SEEK_END);
    size_t length = xftell(file);
    xfseek(file, 0, SEEK_SET);
    return length;
}

static char *valid_file_to_string(FILE *file)
{
    size_t file_size = get_file_size(file);
    char *buffer = xcalloc(file_size + 1, sizeof(char));
    xfread(buffer, 1, file_size, file);
    xfclose(file);
    buffer[file_size] = '\0';
    return buffer;
}

char *file_to_string(const char *filename)
{
    FILE *file = xfopen(filename, "r");
    return valid_file_to_string(file);
}

char *file_fd_to_string(int fd)
{
    FILE *file = xfdopen(fd, "r");
    return valid_file_to_string(file);
}
