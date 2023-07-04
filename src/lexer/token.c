#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dstring.h"
#include "xmalloc.h"
#include "xstring.h"

void destroy_remaining_tokens(struct queue *lexer)
{
    if (lexer)
    {
        struct queue_element *queue_element = lexer->head;
        while (queue_element != NULL)
        {
            token_free_str_and_destroy(queue_element->element);
            queue_element = queue_element->next;
        }
    }
}

struct token *token_create(enum token_type type, char *str, const size_t len)
{
    struct token *token = xcalloc(1, sizeof(struct token));
    token->type = type;
    token->str = xstrndup(str, len);
    return token;
}

void token_destroy(struct token *token)
{
    free(token);
}

void token_free_str_and_destroy(struct token *token)
{
    free(token->str);
    token_destroy(token);
}

struct token *get_next_token(struct queue *lexer)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    return queue_peek(lexer);
}

ssize_t add_single_quote_token(struct queue *lexer, char *input, size_t i)
{
    size_t len = 1;
    while (input[i + len] != '\0' && input[i + len] != '\'')
        len++;
    if (input[i + len] == '\0')
    {
        fprintf(stderr,
                "42sh: "
                "unexpected EOF while looking for matching `''\n");
        return -1;
    }
    struct token *token = token_create(TOKEN_GENERIC, input + i + 1, len - 1);
    token->quote = TOKEN_QUOTE_SINGLE;
    queue_enqueue(lexer, token);
    return len + 1;
}

ssize_t add_partial_quote_token(struct queue *lexer, char *input, size_t i)
{
    size_t len = 1;
    while (input[i + len] != '\0' && input[i + len] != '\"')
        len++;
    if (input[i + len] == '\0')
    {
        fprintf(stderr,
                "42sh: "
                "unexpected EOF while looking for matching `\"'\n");
        return -1;
    }
    struct token *token = token_create(TOKEN_GENERIC, input + i + 1, len - 1);
    token->quote = TOKEN_QUOTE_DOUBLE;
    queue_enqueue(lexer, token);
    return len + 1;
}

static void set_escaped_char(char *input, size_t i)
{
    switch (input[i])
    {
    case '\n':
        input[i] = 'n';
        break;
    case '\a':
        input[i] = 'a';
        break;
    case '\b':
        input[i] = 'b';
        break;
    case '\f':
        input[i] = 'f';
        break;
    case '\r':
        input[i] = 'r';
        break;
    case '\t':
        input[i] = 't';
        break;
    case '\v':
        input[i] = 'v';
        break;
    default:
        break;
    }
}

ssize_t add_escape_quote_token(struct queue *lexer, char *input, size_t i)
{
    size_t len = 1;
    set_escaped_char(input, i + len);
    while (input[i + len] != '\0' && !isspace(input[i + len]))
        len++;
    struct token *token = token_create(TOKEN_GENERIC, input + i + 1, len - 1);
    token->quote = TOKEN_QUOTE_ESCAPE;
    queue_enqueue(lexer, token);
    return len;
}
