#include "parser.h"

#include <stdlib.h>

#include "lexer.h"
#include "skip_newlines.h"
#include "token.h"

struct list_ast *parse(struct queue *lexer, struct hash_map *variables,
                       bool *err_flag)
{
    skip_newlines(lexer);
    struct token *token = queue_peek(lexer);
    struct list_ast *ast = list_parse(lexer, variables);
    if (ast == NULL)
        *err_flag = true;
    skip_newlines(lexer);
    token = queue_peek(lexer);
    if (token->type == TOKEN_EOF)
    {
        token_destroy(token);
        queue_dequeue(lexer);
    }
    return ast;
}
