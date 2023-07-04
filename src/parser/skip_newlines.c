#include "queue.h"
#include "token.h"

void skip_newlines(struct queue *lexer)
{
    struct token *token = queue_peek(lexer);
    while (token->type == TOKEN_NEWLINE)
        token = get_next_token(lexer);
}
