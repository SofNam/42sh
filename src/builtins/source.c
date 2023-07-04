#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "exec.h"
#include "file_handler.h"
#include "token.h"
#include "xfcntl.h"
#include "xmalloc.h"
#include "xstring.h"
#include "xunistd.h"

extern struct global global;

static void get_parameters(int argc, char *const *argv)
{
    char *buf = NULL;
    size_t arg = 1;
    int index = 2;
    while (index < argc)
    {
        buf = xcalloc(digit_count(arg) + 2, sizeof(char));
        sprintf(buf, "%zu", arg++);
        hash_map_insert(global.variables, buf, xstrdup(argv[index]));
        index += 1;
    }
    char *key = xcalloc(2, sizeof(char));
    sprintf(key, "#");
    buf = xcalloc(digit_count(arg) + 2, sizeof(char));
    sprintf(buf, "%zu", arg - 1);
    hash_map_insert(global.variables, key, buf);
}

static int source_work(char *content, int argc, char **argv)
{
    struct global global_save;
    save_global(&global_save);
    global.variables = hash_map_init();
    get_parameters(argc, argv);
    int err = exec(content, global.variables, false);
    restore_global(&global_save);
    free(content);
    return err;
}

static bool has_slash(const char *filename)
{
    return strchr(filename, '/') != NULL;
}

static int open_file_in_path(const char *filename)
{
    char *path = xstrdup(getenv("PATH"));
    char *path_ptr = path;
    if (path == NULL)
        return -1;
    int fd = -1;
    char *token = strtok(path, ":");
    while (token != NULL)
    {
        int dirfd = xopen(token, O_DIRECTORY, 644);
        fd = openat(dirfd, filename, O_RDONLY);
        xclose(dirfd);
        if (fd != -1)
            break;
        token = strtok(NULL, ":");
    }
    free(path_ptr);
    return fd;
}

int source(int argc, char **argv)
{
    if (argc < 2)
    {
        warnx("source: filename argument required");
        warnx("source: usage: source filename [arguments]");
        return 2;
    }
    if (access(argv[1], R_OK) != 0)
    {
        warnx("%s: No such file or directory", argv[1]);
        return 1;
    }
    char *content = NULL;
    if (has_slash(argv[1]))
    {
        content = file_to_string(argv[1]);
    }
    else
    {
        int fd = open_file_in_path(argv[1]);
        if (fd == -1)
        {
            warnx("%s: No such file or directory", argv[1]);
            return 1;
        }
        content = file_fd_to_string(fd);
    }
    return source_work(content, argc, argv);
}
