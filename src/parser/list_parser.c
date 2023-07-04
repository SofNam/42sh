#include <string.h>

#include "destroy_and_nullify.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

void *list_parse(struct queue *lexer, struct hash_map *variables)
{
    struct list_ast *ast = list_ast_create();
    if ((ast->ast = and_or_parse(lexer, variables)) == NULL)
        return destroy_and_nullify(ast, AST_TYPE_LIST);
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_OPERATOR
        && (strcmp(token->str, ";") == 0 || strcmp(token->str, "&") == 0))
    {
        queue_dequeue(lexer);
        token_free_str_and_destroy(token);
        ast->next = list_parse(lexer, variables);
    }
    return ast;
}
