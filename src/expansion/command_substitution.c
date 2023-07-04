#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "dstring.h"
#include "exec.h"
#include "file_handler.h"
#include "string_vector.h"
#include "strstrip.h"
#include "token.h"
#include "vector.h"
#include "xfcntl.h"
#include "xmalloc.h"
#include "xstring.h"
#include "xunistd.h"

#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

extern struct global global;

static char *execute_command(char *command)
{
    int fd =
        xopen("/tmp/42sh_sub", O_CREAT | O_CLOEXEC | O_TRUNC | O_RDWR, PERM);
    int stdout_save = xdup(STDOUT_FILENO);
    xdup2(fd, STDOUT_FILENO);
    struct global global_save;
    save_global(&global_save);
    global.variables = hash_map_init();
    exec(command, global.variables, false);
    restore_global(&global_save);
    xdup2(stdout_save, STDOUT_FILENO);
    xclose(stdout_save);
    xclose(fd);
    char *res = file_to_string("/tmp/42sh_sub");
    free(command);
    return res;
}

static char *substitution_jutsu(char *str, size_t *i)
{
    size_t batman_begins = str[*i] == '`' ? *i + 1 : *i + 2;
    *i = batman_begins;
    while (str[*i] != ')' && str[*i] != '`' && str[*i] != '\0')
        *i += 1;

    if (*i == batman_begins)
        return NULL;

    size_t cmd_len = *i - batman_begins;
    char *command = xstrndup(str + batman_begins, cmd_len);
    command[cmd_len] = '\0';
    return execute_command(command);
}

static void add_fragment(struct string_vector *new_argv,
                         struct vector *new_quote_type, char *fragment,
                         int quote_type)
{
    string_vector_add(new_argv, fragment);
    int *quote = xmalloc(sizeof(int));
    *quote = quote_type;
    vector_add(new_quote_type, quote);
}

static void fragment_substitution(struct string_vector *new_argv,
                                  struct vector *new_quote_type, char *str,
                                  int quote_type)
{
    size_t len = strlen(str);
    size_t start = 0;
    bool reading_chars = false;
    char *fragment = NULL;
    for (size_t i = 0; i < len; i++)
    {
        if (reading_chars)
        {
            if (isspace(str[i]))
            {
                fragment = xstrndup(str + start, i - start);
                add_fragment(new_argv, new_quote_type, fragment, quote_type);
                reading_chars = false;
            }
        }
        else if (!isspace(str[i]))
        {
            reading_chars = true;
            start = i;
        }
    }
    if (reading_chars)
    {
        size_t word_len = str[len - 1] == '\n' ? len - start - 1 : len - start;
        fragment = xstrndup(str + start, word_len);
        add_fragment(new_argv, new_quote_type, fragment, quote_type);
    }
}

static void substitute(struct string_vector *new_argv,
                       struct vector *new_quote_type, char *str, int quote_type)
{
    struct string *string = string_create();
    char *sub = NULL;
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if ((str[i] == '$' && (i == 0 || str[i - 1] != '\\')
             && (str[i + 1] == '('))
            || str[i] == '`')
        {
            sub = substitution_jutsu(str, &i);
            string_add(string, sub);
            free(sub);
        }
        else
        {
            string_add_char(string, str[i]);
        }
    }
    fragment_substitution(new_argv, new_quote_type, string->str, quote_type);
    string_destroy(string);
}

void command_substitution(struct string_vector *new_argv,
                          struct vector *new_quote_type, char *str,
                          int quote_type)
{
    if (str != NULL && (strchr(str, '$') != NULL || strchr(str, '`') != NULL))
        substitute(new_argv, new_quote_type, str, quote_type);
    else
    {
        int *quote = xmalloc(sizeof(int));
        *quote = quote_type;
        string_vector_add(new_argv, xstrdup(str));
        vector_add(new_quote_type, quote);
    }
}
