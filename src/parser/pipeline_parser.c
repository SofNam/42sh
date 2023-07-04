#include <string.h>

#include "destroy_and_nullify.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

static bool pipeline_parse_rec(struct queue *lexer, struct hash_map *variables,
                               struct pipeline_ast *ast)
{
    skip_newlines(lexer);
    struct command_ast *tmp_command = command_parse(lexer, variables);
    if (tmp_command == NULL)
        return false;
    vector_add(ast->commands, tmp_command);
    return true;
}

static void get_exclamation_mark(struct queue *lexer, struct pipeline_ast *ast,
                                 struct token **token)
{
    if ((*token)->type == TOKEN_GENERIC && (strcmp((*token)->str, "!") == 0))
    {
        ast->negate_return = true;
        *token = get_next_token(lexer);
    }
}

void *pipeline_parse(struct queue *lexer, struct hash_map *variables)
{
    struct pipeline_ast *ast = pipeline_ast_create();
    struct token *token = queue_peek(lexer);
    get_exclamation_mark(lexer, ast, &token);
    if (!pipeline_parse_rec(lexer, variables, ast))
        return destroy_and_nullify(ast, AST_TYPE_PIPELINE);
    token = queue_peek(lexer);
    while (token->type == TOKEN_OPERATOR && (strcmp(token->str, "|") == 0))
    {
        queue_dequeue(lexer);
        token_free_str_and_destroy(token);
        if (!pipeline_parse_rec(lexer, variables, ast))
            return destroy_and_nullify(ast, AST_TYPE_PIPELINE);
        token = queue_peek(lexer);
    }
    return ast;
}
