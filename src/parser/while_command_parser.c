#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "destroy_and_nullify.h"
#include "next_is_keyword.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

static bool get_condition(struct queue *lexer, struct hash_map *variables,
                          struct while_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    skip_newlines(lexer);
    ast->condition = compound_list_parse(lexer, variables);
    skip_newlines(lexer);
    token = queue_peek(lexer);
    return (token->type == TOKEN_GENERIC && strcmp(token->str, "do") == 0);
}

static bool get_body(struct queue *lexer, struct hash_map *variables,
                     struct while_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    free(token->str);
    token_destroy(token);
    skip_newlines(lexer);
    ast->body = compound_list_parse(lexer, variables);
    skip_newlines(lexer);
    return ast->body;
}

void *while_command_parse(struct queue *lexer, struct hash_map *variables)
{
    struct while_command_ast *ast = while_command_ast_create();
    if (!get_condition(lexer, variables, ast)
        || !get_body(lexer, variables, ast))
        return destroy_and_nullify(ast, AST_TYPE_WHILE);
    if (next_is_keyword(lexer, "done"))
        return ast;
    return destroy_and_nullify(ast, AST_TYPE_WHILE);
}
