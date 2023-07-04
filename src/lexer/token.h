#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <sys/types.h>

#include "queue.h"

/// @file token.h
/// @brief Contains implementation of tokens
/// and definition of related functions.

/// @brief enum of different types of tokens
enum token_type
{
    TOKEN_NONE,
    TOKEN_GENERIC,
    TOKEN_NEWLINE,
    TOKEN_OPERATOR,
    TOKEN_REDIROP,
    TOKEN_IONUMBER,
    TOKEN_EOF
};

/// Implementation of a token with a type and a string.
struct token
{
    enum token_type type;
    enum token_quote
    {
        TOKEN_QUOTE_NONE,
        TOKEN_QUOTE_SINGLE,
        TOKEN_QUOTE_DOUBLE,
        TOKEN_QUOTE_ESCAPE
    } quote;
    char *str;
};

/// Create a new token with given type and value.
struct token *token_create(enum token_type type, char *str, const size_t len);

/// Deallocate the space allocated to the given token.
void token_destroy(struct token *token);

/// Deallocate the space allocated to the given token
/// and free the string it holds.
void token_free_str_and_destroy(struct token *token);

/// Destroy all remaining tokens.
void destroy_remaining_tokens(struct queue *lexer);

/// Destroy and dequeue current token, return next token
struct token *get_next_token(struct queue *lexer);

/// Add single quoted token, return length of token string
ssize_t add_single_quote_token(struct queue *lexer, char *input, size_t i);

/// Add double quoted token, return length of token string
ssize_t add_partial_quote_token(struct queue *lexer, char *input, size_t i);

/// Add escape quoted token, return length of token string
ssize_t add_escape_quote_token(struct queue *lexer, char *input, size_t i);

#endif /* ! TOKEN_H */
