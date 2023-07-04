#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "hash_map.h"
#include "queue.h"

/// @file parser.h
/// @brief Contains definition of parser.

/// @brief Generate axiom ast from given token list.
/// Set err_flag to true if an error occurred.
/// @return ast if it works else return NULL.
struct list_ast *parse(struct queue *lexer, struct hash_map *variables,
                       bool *err_flag);

/// @biref Generate axiom ast from given token list.
/// @return and_or_ast if it works else return NULL.
void *and_or_parse(struct queue *lexer, struct hash_map *variables);

/// @biref Generate case ast from given token list.
/// @return case_command_ast if it works else return NULL.
void *case_command_parse(struct queue *lexer, struct hash_map *variables);

/// @biref Generate command ast from given token list.
/// @return command_ast if it works else return NULL.
void *command_parse(struct queue *lexer, struct hash_map *variables);

/// @biref Generate compound_list ast from given token compound_list.
/// @return compound_list_ast if it works else return NULL.
void *compound_list_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate for_command_ast from given list of token
/// and set variables if there are assignments.
void *for_command_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate if_command_ast from given list of token
/// and set variables if there are assignments.
void *if_command_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate item_case for case_command
void *item_parse(struct queue *lexer, struct hash_map *variables);

/// @biref Generate list ast from given token list.
/// @return list_ast if it works else return NULL.
void *list_parse(struct queue *lexer, struct hash_map *variables);

/// @biref Generate pipeline ast from given token list.
/// @return pipeline_ast if it works else return NULL.
void *pipeline_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate shell_command_ast from given list of token
/// @return shell_command_ast if it works else return NULL.
void *shell_command_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate simple_command_ast from given list of token
/// and set variables if there are assignments.
/// @return simple_command_ast if it works else return NULL.
void *simple_command_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate while_command_ast from given list of token
/// and set variables if there are assignments.
void *while_command_parse(struct queue *lexer, struct hash_map *variables);

/// @brief Generate until_command_ast from given list of token
/// and set variables if there are assignments.
void *until_command_parse(struct queue *lexer, struct hash_map *variables);

#endif /* ! PARSER_H */
