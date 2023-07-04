#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "builtins.h"
#include "queue.h"
#include "redir.h"
#include "skip_newlines.h"
#include "token.h"
#include "vector.h"
#include "xfcntl.h"
#include "xio.h"
#include "xmalloc.h"
#include "xstring.h"
#include "xunistd.h"

#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

extern struct global global;

int get_largest_io(struct vector *redirs, int **io_save_arr)
{
    struct redir *redir = NULL;
    int largest = -1;
    for (size_t i = 0; i < redirs->size; i++)
    {
        redir = redirs->data[i];
        if (redir->io > largest)
            largest = redir->io;
    }
    *io_save_arr = xcalloc(largest + 1, sizeof(int));
    for (int i = 0; i < largest + 1; i++)
    {
        *((*io_save_arr) + i) = -1;
    }
    return largest;
}

static void store_io_save(int *io_save_arr, struct vector *redirs,
                          size_t current)
{
    struct redir *redir = redirs->data[current];
    if (io_save_arr[redir->io] == -1)
    {
        redir->io_save = xdup(redir->io);
        int index = current;
        io_save_arr[redir->io] = index;
    }
}

static void write_string_from_redir_io(struct redir *redir, FILE *fd)
{
    char *line = NULL;
    size_t size = 0;
    size_t len = xstrlen(redir->word);
    FILE *fd_io = xfdopen(redir->io, "r");
    while (getline(&line, &size, fd_io))
    {
        if (strncmp(line, redir->word, len) == 0 && line[len] == '\n')
            break;
        fputs(line, fd);
    }
    free(line);
    xfclose(fd_io);
}

static void write_string_from_redir_io_dash(struct redir *redir, FILE *fd)
{
    size_t offset = 0;
    char *line = NULL;
    size_t size = 0;
    size_t len = xstrlen(redir->word);
    FILE *fd_io = xfdopen(redir->io, "r");
    while (getline(&line, &size, fd_io))
    {
        offset = 0;
        while (line[offset] == '\t')
            offset++;
        if (strncmp(line + offset, redir->word, len) == 0
            && line[len + offset] == '\n')
            break;
        fputs(line + offset, fd);
    }
    free(line);
    xfclose(fd_io);
}

static void setup_here_doc(struct redir *redir)
{
    FILE *fd = xfopen("/tmp/42sh_here_doc", "w");
    if (redir->type == DLESS)
        write_string_from_redir_io(redir, fd);
    else
        write_string_from_redir_io_dash(redir, fd);
    xfclose(fd);
    redir->fd = xopen("/tmp/42sh_here_doc", O_RDONLY, PERM);
    xunlink("/tmp/42sh_here_doc");
    xdup2(redir->fd, redir->io);
    global.open_redir_fds = 2;
    global.redir_fds[0] = redir->fd;
    global.redir_fds[1] = redir->io;
}

static void set_open_redir_fds(int fd_1, int fd_2, int fd_3)
{
    global.open_redir_fds = 3;
    global.redir_fds[0] = fd_1;
    global.redir_fds[1] = fd_2;
    global.redir_fds[2] = fd_3;
}

void setup_redirections(struct vector *redirs, int *io_save_arr)
{
    struct redir *redir = NULL;
    for (size_t i = 0; i < redirs->size; i++)
    {
        redir = redirs->data[i];
        store_io_save(io_save_arr, redirs, i);
        if (redir->type == DLESS || redir->type == DLESSDASH)
        {
            setup_here_doc(redir);
            continue;
        }
        if (redir->type == GREAT || redir->type == CLOBBER)
            redir->fd = xopen(redir->word, O_WRONLY | O_CREAT | O_TRUNC, PERM);
        else if (redir->type == LESS)
            redir->fd = xopen(redir->word, O_RDONLY, PERM);
        else if (redir->type == LESSGREAT)
            redir->fd = xopen(redir->word, O_RDWR | O_CREAT | O_TRUNC, PERM);
        else if (redir->type == DGREAT)
            redir->fd = xopen(redir->word, O_WRONLY | O_APPEND | O_CREAT, PERM);
        else if (redir->type == GREATAND)
        {
            redir->io_save_err = xdup(STDERR_FILENO);
            redir->fd = xopen(redir->word, O_WRONLY | O_CREAT | O_TRUNC, PERM);
            xdup2(redir->fd, STDERR_FILENO);
        }
        else if (redir->type == LESSAND)
        {
            redir->io_save_err = xdup(STDERR_FILENO);
            redir->fd = atoi(redir->word);
            xdup2(redir->fd, STDERR_FILENO);
        }
        xdup2(redir->fd, redir->io);
        set_open_redir_fds(redir->fd, redir->io, redir->io_save_err);
    }
}

void unredirect(struct vector *redirs, int *io_save_arr, int largest)
{
    if (redirs->size == 0)
        return;
    struct redir *redir = NULL;
    for (int i = 0; i < largest + 1; i++)
    {
        if (io_save_arr[i] != -1)
        {
            redir = redirs->data[io_save_arr[i]];
            xdup2(redir->io_save, redir->io);
        }
    }
    for (size_t i = 0; i < redirs->size; i++)
    {
        redir = redirs->data[i];
        if (redir->type == GREATAND || redir->type == LESSAND)
        {
            xdup2(redir->io_save_err, STDERR_FILENO);
            xclose(redir->io_save_err);
        }
        xclose(redir->fd);
        xclose(redir->io_save);
    }
}
