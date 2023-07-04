#include <stdbool.h>
#include <string.h>

#include "queue.h"
#include "token.h"

bool next_is_keyword(struct queue *lexer, const char *keyword)
{
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_GENERIC && strcmp(token->str, keyword) == 0)
    {
        queue_dequeue(lexer);
        token_free_str_and_destroy(token);
        return true;
    }
    return false;
}
