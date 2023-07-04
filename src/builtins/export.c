#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "name.h"
#include "xstring.h"

#define OVERWRITE_ENV_VARIABLE 1
#define DONT_OVERWRITE_ENV_VARIABLE 0

extern char **environ;
extern struct global global;

static enum export_option filter_option(char c)
{
    if (c == 'n')
        return EXPORT_UNSET_OPTION;
    if (c != 'p')
    {
        fprintf(stderr,
                "42sh: export: -%c: invalid option\nexport: usage: "
                "export [-n] [name[=value] ...] or export -p\n",
                c);
        return EXPORT_INVALID_OPTION;
    }
    return EXPORT_PRINT_OPTION;
}

static enum export_option add_name_and_value_to_env(char *name, size_t name_len)
{
    char *name_cpy = xstrndup(name, name_len);
    char *value = xstrndup(name + name_len + 1, xstrlen(name) - name_len - 1);
    if (!is_valid_name(name_cpy, name_cpy + name_len))
    {
        free(name_cpy);
        return EXPORT_INVALID_NAME;
    }
    value = value == NULL ? "" : value;
    setenv(name_cpy, value, OVERWRITE_ENV_VARIABLE);
    free(name_cpy);
    if (strcmp(value, "") != 0)
        free(value);
    return EXPORT_NO_OPTION;
}

static enum export_option set_name_in_env(char *name, size_t name_len,
                                          const bool unset_name)
{
    char *name_cpy = name_len == 0 ? xstrdup(name) : xstrndup(name, name_len);
    if (!is_valid_name(name_cpy, name_cpy + name_len))
    {
        fprintf(stderr, "42sh: export: `%s': not a valid identifier\n", name);
        free(name_cpy);
        return EXPORT_INVALID_NAME;
    }
    if (unset_name)
        unsetenv(name_cpy);
    else
        setenv(name_cpy, "", DONT_OVERWRITE_ENV_VARIABLE);
    free(name_cpy);
    return EXPORT_NO_OPTION;
}

static enum export_option filter_assignment(char *name, char *equal_sign,
                                            bool *unset_name)
{
    if (equal_sign == name)
        return EXPORT_INVALID_NAME;
    size_t name_last_char_index = xstrlen(name) - 1;
    size_t name_len = name_last_char_index;
    if (equal_sign != name + name_last_char_index)
        name_len = equal_sign - name;
    if (*unset_name)
        return set_name_in_env(name, name_len, *unset_name);
    return add_name_and_value_to_env(name, name_len);
}

static enum export_option filter_name(char *name, bool *unset_name)
{
    char *equal_sign = strchr(name, '=');
    if (equal_sign != NULL)
        return filter_assignment(name, equal_sign, unset_name);
    if (*unset_name)
        return set_name_in_env(name, 0, *unset_name);
    return set_name_in_env(name, 0, *unset_name);
}

static enum export_option filter_argv(char *arg, bool *can_read_option,
                                      bool *unset_name)
{
    enum export_option tmp_opt = EXPORT_PRINT_OPTION;
    enum export_option opt = EXPORT_PRINT_OPTION;
    if (arg[0] != '-' || xstrlen(arg) == 1 || !*can_read_option)
    {
        *can_read_option = false;
        return filter_name(arg, unset_name);
    }
    for (size_t j = 1; arg[j] != '\0'; j++)
    {
        tmp_opt = filter_option(arg[j]);
        if (tmp_opt == EXPORT_INVALID_OPTION)
            return tmp_opt;
        if (tmp_opt == EXPORT_UNSET_OPTION)
            opt = tmp_opt;
    }
    return opt;
}

static void print_env_variables(void)
{
    for (size_t i = 0; environ[i] != NULL; i++)
        printf("export %s\n", environ[i]);
}

int export(int argc, char **argv)
{
    bool print = true;
    bool can_read_option = true;
    bool unset_name = false;
    enum export_option opt = EXPORT_PRINT_OPTION;
    for (int i = 1; i < argc; i++)
    {
        opt = filter_argv(argv[i], &can_read_option, &unset_name);
        if (opt == EXPORT_INVALID_OPTION)
            return 2;
        if (opt == EXPORT_INVALID_NAME)
            return 1;
        if (opt == EXPORT_UNSET_OPTION)
            unset_name = true;
        else if (opt == EXPORT_NO_OPTION)
            print = false;
    }
    if (print)
        print_env_variables();
    return 0;
}
