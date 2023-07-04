#include <stdlib.h>

#include "ast.h"
#include "builtins.h"
#include "exec.h"
#include "expansion.h"
#include "hash_map.h"
#include "name.h"
#include "redir.h"
#include "run_command.h"
#include "string_vector.h"
#include "token.h"
#include "xfcntl.h"
#include "xstring.h"
#include "xunistd.h"

#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

extern struct global global;

static void expand_all_variables(struct simple_command_ast *ast)
{
    for (size_t i = 0; i < ast->quote_type->size; i++)
    {
        int *quote_type = ast->quote_type->data[i];
        if (quote_type != NULL && *quote_type != TOKEN_QUOTE_SINGLE
            && *quote_type != TOKEN_QUOTE_ESCAPE)
            variable_expansion(ast->argv->strings + i, global.variables);
    }
}

static void setup_arg_vector(struct simple_command_ast *ast)
{
    if (ast->argv->size == 0)
        string_vector_add(ast->argv, NULL);
    else if (ast->argv->strings[ast->argv->size - 1] != NULL)
    {
        if (ast->argv->strings[ast->argv->size - 1][0] == ';')
            string_vector_remove(ast->argv);
        string_vector_add(ast->argv, NULL);
    }
}

static int simple_command_exec_with_redir(struct simple_command_ast *ast)
{
    int *io_save_arr = NULL;
    int largest = get_largest_io(ast->redirs, &io_save_arr);
    setup_redirections(ast->redirs, io_save_arr);
    global.last_return = run_command(ast->argv->size - 1, ast->argv->strings);
    unredirect(ast->redirs, io_save_arr, largest);
    free(io_save_arr);
    return global.last_return;
}

static void expand_alias(struct simple_command_ast *ast)
{
    int *quote_type = ast->quote_type->data[0];
    char *name = ast->argv->strings[0];
    if (*quote_type == TOKEN_QUOTE_NONE
        && is_valid_name(name, name + xstrlen(name)))
    {
        char *value = xstrdup(hash_map_get(global.aliases, name));
        if (value != NULL)
        {
            free(ast->argv->strings[0]);
            ast->argv->strings[0] = value;
        }
    }
}

static void command_substitution_all(struct simple_command_ast *ast)
{
    struct string_vector *new_argv = string_vector_create();
    struct vector *new_quote_type = vector_create();
    int *quote_type = NULL;
    for (size_t i = 0; i < ast->argv->size; i++)
    {
        quote_type = ast->quote_type->data[i];
        if (quote_type != NULL && *quote_type != TOKEN_QUOTE_SINGLE)
        {
            command_substitution(new_argv, new_quote_type,
                                 ast->argv->strings[i], *quote_type);
            free(quote_type);
        }
        else
        {
            string_vector_add(new_argv, xstrdup(ast->argv->strings[i]));
            vector_add(new_quote_type, quote_type);
        }
    }
    string_vector_destroy(ast->argv);
    vector_destroy(ast->quote_type);
    ast->argv = new_argv;
    ast->quote_type = new_quote_type;
}

int simple_command_exec(void *ptr)
{
    struct simple_command_ast *ast = ptr;
    if (ast->argv->size == 0)
        return 0;
    expand_alias(ast);
    command_substitution_all(ast);
    if (ast->argv->size == 0)
        return global.last_return;
    struct string_vector *argv_save = string_vector_create();
    for (size_t i = 0; i < ast->argv->size; i++)
    {
        string_vector_add(argv_save, xstrdup(ast->argv->strings[i]));
    }
    expand_all_variables(ast);
    setup_arg_vector(ast);
    int err = simple_command_exec_with_redir(ast);
    string_vector_destroy(ast->argv);
    ast->argv = argv_save;
    return err;
}
