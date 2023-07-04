#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "dstring.h"
#include "expansion.h"
#include "hash_map.h"
#include "name.h"
#include "xstring.h"

static char *expand_variable(char *var, struct hash_map *variables)
{
    if (var == NULL)
        return "";
    if (strcmp(var, "$") == 0)
        return expand_dollar();
    if (strcmp(var, "UID") == 0)
        return expand_uid();
    if (strcmp(var, "?") == 0)
        return expand_qmark();
    if (strcmp(var, "RANDOM") == 0)
        return expand_random();
    if (strcmp(var, "@") == 0 || strcmp(var, "*") == 0)
        return expand_all_parameters(variables);
    char *value = hash_map_get(variables, var);
    if (value != NULL)
        return xstrdup(value);
    value = getenv(var);
    return value == NULL ? "" : xstrdup(value);
}

static char *get_special(char *str, size_t *i)
{
    if (str[*i] == '#' || str[*i] == '$' || str[*i] == '?' || str[*i] == '*'
        || str[*i] == '@' || isdigit(str[*i]))
        return xstrndup(str + *i, 1);
    return NULL;
}

static char *get_name_brace(char *str, size_t *i)
{
    size_t start = *i + 2;
    *i = start;
    while (str[*i] != '}' && is_valid_char(str[*i]))
        *i += 1;
    if (*i == start)
        return NULL;
    if (str[*i] == '}')
    {
        char *dup = xstrndup(str + start, *i - start);
        return dup;
    }
    char *dup = xstrndup(str + start, *i - start);
    fprintf(stderr, "42sh: %s: bad susbstitution \n", dup);
    free(dup);
    char *exit_code[3] = { NULL, "1", NULL };
    xexit(2, exit_code);
    // Code never reached
    return NULL;
}

static char *get_name(char *str, size_t *i)
{
    size_t start = *i + 1;
    *i = start;
    char *dup = get_special(str, i);
    if (dup)
        return dup;
    while (is_valid_char(str[*i]))
        *i += 1;
    if (*i == start)
        return NULL;
    dup = xstrndup(str + start, *i - start);
    *i -= 1;
    return dup;
}

static bool add_if_backslash(char *str, struct string *string, size_t *i)
{
    if (str[*i + 1] != '\0')
    {
        if (str[*i + 1] == '$')
        {
            *i = *i + 1;
            string_add_char(string, str[*i]);
            return true;
        }
        string_add_char(string, '\\');
        if (str[*i + 1] != '\\')
        {
            string_add_char(string, str[*i + 1]);
            *i = *i + 1;
            return true;
        }
        *i = *i + 1;
        return true;
    }
    string_add_char(string, str[*i]);
    return false;
}

static char *replace_var_in_string(char *str, struct hash_map *variables)
{
    struct string *string = string_create();
    char *var = NULL;
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\\' && add_if_backslash(str, string, &i))
            continue;
        if (str[i] == '$')
        {
            var =
                str[i + 1] == '{' ? get_name_brace(str, &i) : get_name(str, &i);
            char *buf = expand_variable(var, variables);
            if (strcmp(buf, "") != 0)
            {
                string_add(string, buf);
                free(buf);
            }
            free(var);
        }
        else
        {
            string_add_char(string, str[i]);
        }
    }
    char *expand_str = xstrdup(string->str);
    string_destroy(string);
    return expand_str;
}

void variable_expansion(char **str, struct hash_map *variables)
{
    if (strchr(*str, '$') != NULL)
    {
        char *expand_str = replace_var_in_string(*str, variables);
        free(*str);
        *str = expand_str;
    }
}
