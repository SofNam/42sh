#ifndef AST_TO_DOT_H
#define AST_TO_DOT_H

#include <stdio.h>

#include "ast.h"

/// @file ast_to_dot.h
/// @brief Definition of function to create a pretty print for the ASTs.

/// @brief Create a dot format file from the given list_ast
/// at the root of the repository: ./ast.dot
void ast_to_dot(struct list_ast *ast);

void list_ast_to_dot(FILE *file, struct list_ast *ast);

void and_or_ast_to_dot(FILE *file, void *ptr, int parent_id);

void case_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void compound_list_to_dot(FILE *file, void *ptr, int parent_id);

void for_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void if_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void pipeline_ast_to_dot(FILE *file, void *ptr, int parent_id);

void shell_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void simple_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void until_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

void while_command_ast_to_dot(FILE *file, void *ptr, int parent_id);

#endif /* ! AST_TO_DOT_H */
