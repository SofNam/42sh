#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "name.h"
#include "xstring.h"

extern struct global global;

static int set_alias(char *arg, size_t arg_len)
{
    char *arg_cpy = xstrndup(arg, arg_len);
    char *value = xstrndup(arg + arg_len + 1, xstrlen(arg) - arg_len - 1);
    if (!is_valid_name(arg_cpy, arg_cpy + arg_len))
    {
        free(arg_cpy);
        return 1;
    }
    hash_map_insert(global.aliases, arg_cpy, value);
    return 0;
}

int filter_alias_assignment(char *arg, char *equal_sign)
{
    if (equal_sign == arg)
        return 1;
    size_t arg_last_char_index = xstrlen(arg) - 1;
    size_t arg_len = arg_last_char_index;
    if (equal_sign != arg + arg_last_char_index)
        arg_len = equal_sign - arg;
    else
        arg_len--;
    return set_alias(arg, arg_len);
}

static int filter_alias(char *arg)
{
    char *equal_sign = strchr(arg, '=');
    if (equal_sign == NULL)
    {
        char *value = hash_map_get(global.aliases, arg);
        if (value == NULL)
        {
            warnx("alias: %s: not found", arg);
            return 1;
        }
        printf("alias %s='%s'\n", arg, value);
        return 0;
    }
    return filter_alias_assignment(arg, equal_sign);
}

int alias(int argc, char **argv)
{
    optind = 1;
    bool print = false;
    if (argc == 1)
    {
        hash_map_dump(global.aliases, print);
        return 0;
    }
    int option;
    while ((option = getopt(argc, argv, ":p")) != -1)
    {
        switch (option)
        {
        case 'p':
            print = true;
            break;
        default:
            warnx("alias: -%c: invalid option\n alias: usage: alias [-p] "
                  "[name[=value]]",
                  optopt);
            return 2;
        }
    }
    if (print)
        hash_map_dump(global.aliases, print);
    int err = 0;
    for (; optind < argc; optind++)
        err |= filter_alias(argv[optind]);
    return err;
}
