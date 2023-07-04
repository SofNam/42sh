#ifndef NEXT_IS_KEYWORD_H
#define NEXT_IS_KEYWORD_H

#include <stdbool.h>

#include "queue.h"
/// @file next_is_keyword.h
/// @brief Definition of a function to check if next token is right keyword.

/// @brief
/// returns true if the head of lexer is the given keyword and
/// dequeue and free the head
bool next_is_keyword(struct queue *lexer, const char *keyword);

#endif /* ! NEXT_IS_KEYWORD_H */
