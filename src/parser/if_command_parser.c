#include <stdbool.h>
#include <string.h>

#include "destroy_and_nullify.h"
#include "next_is_keyword.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

static bool get_condition(struct queue *lexer, struct hash_map *variables,
                          struct if_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    ast->condition = compound_list_parse(lexer, variables);
    if (ast->condition == NULL)
        return false;
    token = queue_peek(lexer);
    return token->type == TOKEN_GENERIC && strcmp(token->str, "then") == 0;
}

static bool get_if_body(struct queue *lexer, struct hash_map *variables,
                        struct if_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    ast->if_body = compound_list_parse(lexer, variables);
    return ast->if_body != NULL;
}

static bool get_else_body(struct queue *lexer, struct hash_map *variables,
                          struct if_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    ast->type = ELSE;
    ast->else_body = compound_list_parse(lexer, variables);
    if (!next_is_keyword(lexer, "fi"))
        return false;
    return ast->else_body != NULL;
}

static bool get_elif(struct queue *lexer, struct hash_map *variables,
                     struct if_command_ast *ast)
{
    ast->type = ELIF;
    ast->else_body = if_command_parse(lexer, variables);
    return ast->else_body != NULL;
}

void *if_command_parse(struct queue *lexer, struct hash_map *variables)
{
    struct if_command_ast *ast = if_command_ast_create();
    if (!get_condition(lexer, variables, ast)
        || !get_if_body(lexer, variables, ast))
        return destroy_and_nullify(ast, AST_TYPE_IF);
    if (next_is_keyword(lexer, "fi"))
        return ast;
    struct token *token = queue_peek(lexer);
    if (token->type != TOKEN_GENERIC)
        return destroy_and_nullify(ast, AST_TYPE_IF);
    if (strcmp(token->str, "else") == 0)
    {
        if (get_else_body(lexer, variables, ast))
            return ast;
        return destroy_and_nullify(ast, AST_TYPE_IF);
    }
    if (token->type != TOKEN_GENERIC)
        return destroy_and_nullify(ast, AST_TYPE_IF);
    if (strcmp(token->str, "elif") == 0 && get_elif(lexer, variables, ast))
        return ast;
    return destroy_and_nullify(ast, AST_TYPE_IF);
}
