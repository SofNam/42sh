#include "option.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xmalloc.h"
#include "xstring.h"

static bool is_shopt(const char *shopt)
{
    return (strcmp(shopt, "dotglob") == 0)
        || (strcmp(shopt, "expand_aliases") == 0)
        || (strcmp(shopt, "extglob") == 0) || (strcmp(shopt, "nocaseglob") == 0)
        || (strcmp(shopt, "nullglob") == 0)
        || (strcmp(shopt, "sourcepath") == 0)
        || (strcmp(shopt, "xpg_echo") == 0)
        || (strcmp(shopt, "ast-print") == 0);
}

static int check_O(struct option *option, int argc, char *const *argv,
                   int *index)
{
    bool minus = strcmp(argv[*index], "-O") == 0;
    struct option_O *new = get_last_O(option);
    new->state = minus ? -1 : 1;
    new->position = *index;
    if ((*index + 1) < argc)
    {
        if (!is_shopt(argv[*index + 1]))
        {
            char *buf = xcalloc(xstrlen(argv[*index + 1]) + 40, sizeof(char));
            sprintf(buf, "%s: invalid shell option name", argv[*index + 1]);
            option->error = buf;
            return -1;
        }
        new->shopt = argv[*index + 1];
        *index += 1;
    }
    *index += 1;
    return 0;
}

static void check_O_wrapper(struct option *option, int argc, char *const *argv,
                            int *index)
{
    int res;
    if (*index < argc && xstrlen(argv[*index]) == 2)
    {
        if ((strncmp(argv[*index], "-", 1) == 0
             || strncmp(argv[*index], "+", 1) == 0)
            && argv[*index][1] == 'O')
        {
            res = check_O(option, argc, argv, index);
            if (res == -1 || res == 0)
                return;
        }
        else
        {
            char *buf = xcalloc(xstrlen(argv[*index]) + 40, sizeof(char));
            sprintf(buf, "%s: invalid option", argv[*index]);
            option->error = buf;
        }
    }
}

static void get_parameters(int argc, char *const *argv, int *index,
                           struct hash_map *variables)
{
    char *buf = NULL;
    size_t arg = 1;
    while (*index < argc)
    {
        buf = xcalloc(digit_count(arg) + 2, sizeof(char));
        sprintf(buf, "%zu", arg++);
        hash_map_insert(variables, buf, xstrdup(argv[*index]));
        *index += 1;
    }
    char *key = xcalloc(2, sizeof(char));
    sprintf(key, "#");
    buf = xcalloc(digit_count(arg) + 2, sizeof(char));
    sprintf(buf, "%zu", arg - 1);
    hash_map_insert(variables, key, buf);
}

static void check_c(struct option *option, int argc, char *const *argv,
                    int *index, struct hash_map *variables)
{
    if (*index < argc && xstrlen(argv[*index]) == 2
        && strcmp(argv[*index], "-c") == 0 && option->c->state == 0)
    {
        option->c->state = 1;
        option->c->position = *index;
        if ((*index + 1) < argc)
        {
            option->c->command = argv[*index + 1];
            *index += 2;
            char *buf = xcalloc(2, sizeof(char));
            sprintf(buf, "0");
            if (*index == argc)
                hash_map_insert(variables, buf, xstrdup(argv[0]));
            else
            {
                hash_map_insert(variables, buf, xstrdup(argv[*index]));
                *index += 1;
            }
            get_parameters(argc, argv, index, variables);
        }
        else
        {
            char *buf = xcalloc(40, sizeof(char));
            sprintf(buf, "-c: option requires an argument");
            option->error = buf;
        }
    }
}

static void check_filename(struct option *option, int argc, char *const *argv,
                           int *index, struct hash_map *variables)
{
    if (*index < argc)
    {
        option->filename->filename = argv[*index];
        option->filename->position = *index;
        *index += 1;
        char *buf = xcalloc(2, sizeof(char));
        sprintf(buf, "0");
        hash_map_insert(variables, buf, xstrdup(option->filename->filename));
        get_parameters(argc, argv, index, variables);
    }
}

static int is_gnu_long_option(const char *gnu_long_option)
{
    char *gnu_options[14] = { "--debugger",     "--dump-po-strings",
                              "--dump-strings", "--help",
                              "--init-file",    "--rcfile",
                              "--login",        "--noediting",
                              "--noprofile",    "--norc",
                              "--posix",        "--restricted",
                              "--verbose",      "--version" };

    for (int i = 0; i < 14; i++)
    {
        if (strcmp(gnu_long_option, gnu_options[i]) == 0)
            return i;
    }

    if (strncmp(gnu_long_option, "--", 2) == 0)
        return -1;

    return -2;
}

static void check_gnu_long_option(struct option *option, int argc,
                                  char *const *argv, int *index)
{
    if (*index < argc && option->gnu->e_gnu == NO_OPTION)
    {
        int i = is_gnu_long_option(argv[*index]);
        if (i >= 0)
        {
            option->gnu->e_gnu = i;
            option->gnu->position = *index;
            *index += 1;
        }

        else if (i == -1)
        {
            char *buf = xcalloc(xstrlen(argv[*index]) + 40, sizeof(char));
            sprintf(buf, "%s: invalid option", argv[*index]);
            option->error = buf;
        }

        return;
    }
}

struct option *get_option(int argc, char *const *argv,
                          struct hash_map *variables)
{
    struct option *option = option_init();
    int index = 1;

    if (argc == 1)
        return option;

    while (index < argc)
    {
        int temp = index;
        check_gnu_long_option(option, argc, argv, &index);
        if (option->error)
            break;

        if (temp != index)
            continue;

        check_c(option, argc, argv, &index, variables);
        if (option->error || option->c->state == 1)
            break;

        check_O_wrapper(option, argc, argv, &index);
        if (option->error)
            break;

        if (temp != index)
            continue;

        check_filename(option, argc, argv, &index, variables);
        if (option->filename->filename)
            break;
    }
    return option;
}
