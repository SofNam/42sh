#include <stdlib.h>
#include <string.h>

#include "expansion.h"
#include "xmalloc.h"
#include "xstring.h"

struct dirs
{
    char *home;
    char *oldpwd;
    size_t home_len;
    size_t oldpwd_len;
};

static void init_dirs(struct dirs *dirs)
{
    dirs->home = getenv("HOME");
    dirs->oldpwd = getenv("OLDPWD");
    dirs->home_len = xstrlen(dirs->home);
    dirs->oldpwd_len = xstrlen(dirs->oldpwd);
}

static size_t get_expansion_len(char *str, size_t home_len, size_t oldpwd_len)
{
    size_t len = 0;
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '~')
        {
            len++;
            continue;
        }
        if (str[i + 1] == '-')
        {
            len += oldpwd_len;
            i++;
        }
        else if (str[i + 1] == '+')
        {
            len++;
            i++;
        }
        else
        {
            len += home_len;
        }
    }
    return len + 1;
}

static char *replace_tilde_in_string(char *str, const struct dirs dirs)
{
    size_t new_len = get_expansion_len(str, dirs.home_len, dirs.oldpwd_len);
    char *expand_str = xcalloc(new_len, sizeof(char));
    size_t j = 0;
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '~')
        {
            expand_str[j++] = str[i];
            continue;
        }
        if (str[i + 1] == '-')
        {
            strcat(expand_str + j, dirs.oldpwd);
            j += dirs.oldpwd_len;
            i++;
        }
        else if (str[i + 1] == '+')
        {
            expand_str[j++] = '.';
            i++;
        }
        else
        {
            strcat(expand_str + j, dirs.home);
            j += dirs.home_len;
        }
    }
    return expand_str;
}

void tilde_expansion(struct token *token)
{
    if (token->quote == TOKEN_QUOTE_NONE && strchr(token->str, '~') != NULL)
    {
        struct dirs dirs;
        init_dirs(&dirs);
        char *expand_str = replace_tilde_in_string(token->str, dirs);
        free(token->str);
        token->str = expand_str;
    }
}
