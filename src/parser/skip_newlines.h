#ifndef SKIP_NEWLINES_H
#define SKIP_NEWLINES_H

/// @file skip_newlines.h
/// @brief Contains definition of skip_newline function.

/// @brief Skips all the new_line tokens until
/// finding a different kind of token.
void skip_newlines(struct queue *lexer);

#endif /* ! SKIP_NEWLINES_H */
