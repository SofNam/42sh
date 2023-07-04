#include "exec.h"

#include <stdbool.h>
#include <stdio.h>

#include "ast_to_dot.h"
#include "builtins.h"
#include "hash_map.h"
#include "lexer.h"
#include "parser.h"
#include "queue.h"
#include "skip_newlines.h"
#include "token.h"

extern struct global global;

static int exec_core(struct queue *lexer, struct hash_map *variables,
                     bool *parsing_err_flag, bool should_print_ast)
{
    bool err_flag = false;
    struct list_ast *ast = parse(lexer, variables, &err_flag);
    global.ast = ast;
    if (should_print_ast)
        ast_to_dot(ast);
    if (ast == NULL)
    {
        if (err_flag)
        {
            *parsing_err_flag = true;
            fprintf(stderr, "42sh: syntax error\n");
            fflush(stderr);
            return 2;
        }
        fflush(stderr);
        return 0;
    }
    int err = list_exec(ast);
    fflush(stdout);
    fflush(stderr);
    list_ast_destroy(ast);
    return err;
}

int exec(char *input, struct hash_map *variables, bool should_print_ast)
{
    global.aliases = hash_map_init();
    struct queue *lexer = lex(input);
    if (lexer == NULL)
        return 2;
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_EOF)
    {
        destroy_remaining_tokens(lexer);
        queue_destroy(lexer);
        return 0;
    }
    global.lexer = lexer;
    int err = 0;
    bool parsing_err_flag = false;
    while (queue_peek(lexer) != NULL && !parsing_err_flag)
    {
        skip_newlines(lexer);
        err = exec_core(lexer, variables, &parsing_err_flag, should_print_ast);
    }
    destroy_remaining_tokens(lexer);
    queue_destroy(lexer);
    return err;
}
