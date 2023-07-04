#ifndef EXPANSION_H
#define EXPANSION_H

#include "hash_map.h"
#include "string_vector.h"
#include "token.h"
#include "vector.h"

/// @file expansion.h
/// @brief Contains definition of expansion functions

char *expand_dollar(void);
char *expand_uid(void);
char *expand_qmark(void);
char *expand_random(void);
char *expand_all_parameters(struct hash_map *variables);

/// Expand all '~' characters in token string
void tilde_expansion(struct token *token);

/// Expand all variables in token string
void variable_expansion(char **str, struct hash_map *variables);

/// Substitute commands in str and add them to new_argv
void command_substitution(struct string_vector *new_argv,
                          struct vector *new_quote_type, char *str,
                          int quote_type);

#endif /* ! EXPANSION_H */
