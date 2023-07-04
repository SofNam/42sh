#include <err.h>
#include <stdio.h>
#include <unistd.h>

#include "builtins.h"
#include "xstring.h"

extern struct global global;

static int invalid_option(char c)
{
    warnx("unalias: -%c: invalid option", c);
    fprintf(stderr, "unalias: usage: unalias [-a] name [name ...]\n");
    return 2;
}

static int unalias_all(void)
{
    hash_map_destroy(global.aliases);
    global.aliases = hash_map_init();
    return 0;
}

static int unalias_alias(char *alias)
{
    if (!hash_map_remove(global.aliases, alias))
    {
        warnx("unalias: %s: not found", alias);
        return 1;
    }
    return 0;
}

static bool filter_option(char *arg, bool *unset_all)
{
    for (size_t i = 1; arg[i] != '\0'; i++)
    {
        if (arg[i] == 'a')
            *unset_all = true;
        else
            return invalid_option(arg[i]);
    }
    return 0;
}

int unalias(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "unalias: usage: unalias [-a] name [name ...]\n");
        return 2;
    }
    bool unset_all = false;
    int arg = 1;
    if (argv[arg][0] == '-' && xstrlen(argv[arg]) > 1)
    {
        int err = filter_option(argv[arg++], &unset_all);
        if (err != 0)
            return err;
    }
    if (unset_all)
        return unalias_all();
    int err = 0;
    for (; arg < argc; arg++)
        err |= unalias_alias(argv[arg]);
    return err;
}
