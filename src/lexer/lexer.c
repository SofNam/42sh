#include "lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "builtins.h"
#include "name.h"
#include "queue.h"
#include "token.h"
#include "xstring.h"

static bool is_token_delim(const char c)
{
    return c == ';' || c == '|' || c == '&' || c == '}' || isspace(c)
        || c == '#' || c == '(' || c == ')' || c == '{' || c == '\0' || c == '>'
        || c == '<' || c == '\"' || c == '\'' || c == '\\';
}

static size_t get_next_newline_dist(char *input, const size_t i)
{
    size_t dist = 0;
    while (input[i + dist] != '\0' && input[i + dist] != '\n')
        dist++;
    return dist;
}

static size_t less_redirop(struct queue *lexer, char *input, const size_t i)
{
    if (input[i + 1] == '<')
    {
        if (input[i + 2] == '-')
        {
            queue_enqueue(lexer, token_create(TOKEN_REDIROP, input + i, 3));
            return 3;
        }
        else
        {
            queue_enqueue(lexer, token_create(TOKEN_REDIROP, input + i, 2));
            return 2;
        }
    }
    else if (input[i + 1] == '&' || input[i + 1] == '>')
    {
        queue_enqueue(lexer, token_create(TOKEN_REDIROP, input + i, 2));
        return 2;
    }
    queue_enqueue(lexer, token_create(TOKEN_REDIROP, input + i, 1));
    return 1;
}

static size_t great_redirop(struct queue *lexer, char *input, const size_t i)
{
    if (input[i + 1] == '>' || input[i + 1] == '|' || input[i + 1] == '&')
    {
        queue_enqueue(lexer, token_create(TOKEN_REDIROP, input + i, 2));
        return 2;
    }
    queue_enqueue(lexer, token_create(TOKEN_REDIROP, input + i, 1));
    return 1;
}

static size_t add_duplicatable_delimiters(struct queue *lexer, char *input,
                                          const size_t i)
{
    if (input[i + 1] == input[i])
    {
        queue_enqueue(lexer, token_create(TOKEN_OPERATOR, input + i, 2));
        return 2;
    }
    queue_enqueue(lexer, token_create(TOKEN_OPERATOR, input + i, 1));
    return 1;
}

static size_t add_token_delim(struct queue *lexer, char *input, const size_t i)
{
    if (input[i] == '\0')
        return 0;
    if (input[i] == '\\')
        return add_escape_quote_token(lexer, input, i);
    if (input[i] == '\'')
        return add_single_quote_token(lexer, input, i);
    if (input[i] == '\"')
        return add_partial_quote_token(lexer, input, i);
    if (input[i] == '\n')
    {
        queue_enqueue(lexer, token_create(TOKEN_NEWLINE, NULL, 0));
        return 1;
    }
    if (input[i] == ';' || input[i] == '&' || input[i] == '|')
        return add_duplicatable_delimiters(lexer, input, i);
    if (input[i] == '(' || input[i] == ')' || input[i] == '{'
        || input[i] == '}')
    {
        queue_enqueue(lexer, token_create(TOKEN_OPERATOR, input + i, 1));
        return 1;
    }
    if (input[i] == '#')
        return get_next_newline_dist(input, i);
    if (input[i] == '<')
        return less_redirop(lexer, input, i);
    if (input[i] == '>')
        return great_redirop(lexer, input, i);
    return 1;
}

static void read_until_closing_char(char **input, const size_t i, char c)
{
    if (*(*input + i) == '`')
        *input += 1;
    while (*(*input + i) != '\0' && *(*input + i) != c)
        *input += 1;
    if (*(*input + i) == '\0')
    {
        fprintf(stderr,
                "42sh: unexpected EOF "
                "while looking for matching `%c'\n",
                c);
        char *exit_code[3] = { NULL, "2", NULL };
        xexit(2, exit_code);
    }
}

static char *get_token_end(char *input, const size_t i)
{
    size_t iter = 0;
    while (!is_token_delim(input[i])
           || ((input[i] == '#' || input[i] == '{' || input[i] == '(')
               && ((i > 0 && input[i - 1] == '$')
                   || (iter > 0 && (input - 1)[i] == '$'))))
    {
        if (input[i] == '{')
            read_until_closing_char(&input, i, '}');
        else if (input[i] == '(')
            read_until_closing_char(&input, i, ')');
        else if (input[i] == '`')
            read_until_closing_char(&input, i, '`');
        input++;
        iter++;
    }
    return input + i;
}

static size_t add_token(struct queue *lexer, char *input, const size_t i)
{
    char *token_end = get_token_end(input, i);
    size_t len = token_end - (input + i);
    if (len == 0)
        return add_token_delim(lexer, input, i);
    else if (*token_end == '>' || *token_end == '<')
        queue_enqueue(lexer, token_create(TOKEN_IONUMBER, input + i, len));
    else
        queue_enqueue(lexer, token_create(TOKEN_GENERIC, input + i, len));
    return len;
}

struct queue *lex(char *input)
{
    struct queue *lexer = queue_create();
    ssize_t token_len;
    size_t i = 0;
    while (input[i] != '\0')
    {
        token_len = add_token(lexer, input, i);
        if (token_len == -1)
        {
            destroy_remaining_tokens(lexer);
            queue_destroy(lexer);
            return NULL;
        }
        i += token_len;
    }
    queue_enqueue(lexer, token_create(TOKEN_EOF, NULL, 0));
    return lexer;
}
