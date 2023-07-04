#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast_to_dot.h"
#include "builtins.h"
#include "exec.h"
#include "file_handler.h"
#include "lexer.h"
#include "option.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"
#include "xio.h"
#include "xmalloc.h"
#include "xstring.h"

struct global global;

static void set_ifs(struct hash_map *variables)
{
    char *key = xcalloc(4, sizeof(1));
    sprintf(key, "IFS");
    char *val = xcalloc(4, sizeof(1));
    sprintf(val, " \t\n");
    hash_map_insert(variables, key, val);
}

static struct option *get_option_struct(int argc, char **argv,
                                        struct hash_map *variables)
{
    struct option *option = get_option(argc, argv, variables);
    if (option->error)
    {
        fprintf(stderr, "42sh: %s\n", option->error);
        free_option(option);
        exit(1);
    }
    return option;
}

static int while_stdin(struct hash_map *variables)
{
    int err = 0;
    char *currline = NULL;
    size_t len = 0;
    fprintf(stderr, "42sh$ ");
    while (getline(&currline, &len, stdin) != EOF)
    {
        err = exec(currline, variables, false);
        fprintf(stderr, "42sh$ ");
    }
    fprintf(stderr, "\n");
    hash_map_destroy(variables);
    free(currline);
    return err;
}

static int pipe_stdin(struct hash_map *variables)
{
    int err = 0;
    char *currline = NULL;
    size_t len = 0;
    char *stdin_str = xcalloc(1, sizeof(char));
    while (getline(&currline, &len, stdin) != EOF)
    {
        stdin_str = xrealloc(stdin_str, xstrlen(stdin_str) + len + 1);
        strcat(stdin_str, currline);
    }
    err = exec(stdin_str, variables, false);
    hash_map_destroy(variables);
    free(currline);
    free(stdin_str);
    return err;
}

static int read_from_stdin(struct hash_map *variables)
{
    if (isatty(fileno(stdin)) == 0)
        return pipe_stdin(variables);
    return while_stdin(variables);
}

static bool should_print_ast(struct option *option)
{
    return option->O->state && strcmp(option->O->shopt, "ast-print") == 0;
}

int main(int argc, char **argv)
{
    global.open_fds = 0;
    global.open_redir_fds = 0;
    int err = 0;
    global.variables = hash_map_init();
    set_ifs(global.variables);
    if (argc == 1)
    {
        err = read_from_stdin(global.variables);
        return err;
    }
    struct option *option = get_option_struct(argc, argv, global.variables);
    if (option->c->state == 1)
    {
        err = exec(option->c->command, global.variables,
                   should_print_ast(option));
    }
    else if (option->filename->filename != NULL)
    {
        char *input = file_to_string(option->filename->filename);
        err = exec(input, global.variables, should_print_ast(option));
        free(input);
    }
    hash_map_destroy(global.variables);
    hash_map_destroy(global.aliases);
    free_option(option);
    return err;
}
