#ifndef EXEC_H
#define EXEC_H

#include <stdbool.h>

#include "ast.h"
#include "hash_map.h"

/// @file exec.h
/// @brief Execute content of the input
/// and return the exit status of the execution.
int exec(char *input, struct hash_map *variables, bool should_print_ast);

/// @brief Execute content of the case_command_ast
/// and return the exit status of the execution.
int case_command_exec(void *ptr);

/// @brief Execute content of the and_or_ast
/// and return the exit status of the execution.
int and_or_exec(void *ptr);

/// @brief Execute content of the command_ast
/// and return the exit status of the execution.
int command_exec(void *ptr);

/// @brief Execute content of the compound_list_ast
/// and return the exit status of the execution.
int compound_list_exec(void *ptr);

/// @brief Execute content of the for_command_ast
/// and return the exit status of the execution.
int for_command_exec(void *ptr);

/// @brief Execute content of the if_command_ast
/// and return the exit status of the execution.
int if_command_exec(void *ptr);

/// @brief Execute content of the list_ast
/// and return the exit status of the execution.
int list_exec(struct list_ast *ast);

/// @brief Execute content of the pipeline_ast
/// and return the exit status of the execution.
int pipeline_exec(void *ptr);

/// Get the largest IO number in redirs and allocate io_save_arr
int get_largest_io(struct vector *redirs, int **io_save_arr);

/// Setup redirections before command execution
void setup_redirections(struct vector *redirs, int *io_save_arr);

/// Reset all file descriptors following command execution
void unredirect(struct vector *redirs, int *io_save_arr, int largest);

/// @brief Execute content of the shell_command_ast
/// and return the exit status of the execution.
int shell_command_exec(void *ptr);

/// @brief Execute content of the simple_command_ast
/// and return the exit status of the execution.
int simple_command_exec(void *ptr);

/// @brief Execute content of the until_command_ast
/// and return the exit status of the execution.
int until_command_exec(void *ptr);

/// @brief Execute content of the while_command_ast
/// and return the exit status of the execution.
int while_command_exec(void *ptr);

#endif /* ! EXEC_H */
