#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "string_vector.h"

static char *get_dst_dir(int argc, char **argv, bool *goback)
{
    if (argc == 1)
        return getenv("HOME");
    if (strcmp(argv[1], "-") == 0)
    {
        *goback = true;
        return getenv("OLDPWD");
    }
    return argv[1];
}

static void set_oldpwd(void)
{
    char *cwd = gwd();
    setenv("OLDPWD", cwd, 1);
    free(cwd);
}

int cd(int argc, char **argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "42sh: cd: too many arguments\n");
        return 1;
    }
    bool goback = false;
    char *destination = get_dst_dir(argc, argv, &goback);
    if (destination == NULL)
        return 1;
    set_oldpwd();
    if (chdir(destination) == -1)
    {
        if (errno == EACCES)
            warnx("cd: %s: Permission denied", destination);
        else if (errno == ENOENT)
            warnx("cd: %s: No such file or directory", destination);
        else if (errno == ENOTDIR)
            warnx("cd: %s: Not a directory", destination);
        return 1;
    }
    if (goback)
        pwd();
    return 0;
}
