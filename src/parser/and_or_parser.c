#include <string.h>

#include "destroy_and_nullify.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

void *and_or_parse(struct queue *lexer, struct hash_map *variables)
{
    struct and_or_ast *ast = and_or_ast_create();
    if ((ast->ast = pipeline_parse(lexer, variables)) == NULL)
        return destroy_and_nullify(ast, AST_TYPE_AND_OR);
    struct token *token = queue_peek(lexer);
    if (token->type != TOKEN_OPERATOR)
        return ast;
    if (strcmp(token->str, "&&") == 0)
        ast->type = AND;
    else if (strcmp(token->str, "||") == 0)
        ast->type = OR;
    else
        return ast;
    token_free_str_and_destroy(token);
    queue_dequeue(lexer);
    skip_newlines(lexer);
    if ((ast->next = and_or_parse(lexer, variables)) == NULL)
        return destroy_and_nullify(ast, AST_TYPE_AND_OR);
    return ast;
}
