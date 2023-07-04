#ifndef LEXER_H
#define LEXER_H

#include "queue.h"

/// @file lexer.h
/// @brief Contains the implementation of the lexer.

/// @brief Generate queue of token from given input string.
struct queue *lex(char *input);

#endif /* ! LEXER_H */
