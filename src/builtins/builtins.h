#ifndef BUILTINS_H
#define BUILTINS_H

#include <stddef.h>

#include "ast.h"
#include "hash_map.h"
#include "queue.h"

/// @file builtins.h
/// @brief Implementation of the required shell builtins.

/// @brief global struct
struct global
{
    int last_return;
    size_t open_fds;
    int fds[2];
    size_t open_redir_fds;
    int redir_fds[3];
    struct list_ast *ast;
    struct queue *lexer;
    struct hash_map *variables;
    struct hash_map *aliases;
    int cont;
    int nested_loop;
    int continue_err;
    int breaky;
    int breaky_err;
};

enum export_option
{
    EXPORT_INVALID_OPTION,
    EXPORT_INVALID_NAME,
    EXPORT_UNSET_OPTION,
    EXPORT_PRINT_OPTION,
    EXPORT_NO_OPTION
};

/// @brief alias builtin function
int alias(int argc, char **argv);

/// @brief unalias builtin function
int unalias(int argc, char **argv);

/// @brief Change the working directory.
int cd(int argc, char **argv);

/// @brief Return the current working directory.
char *gwd(void);

/// @brief Print name of current/working directory.
int pwd(void);

/// @brief echo builtin function
int echo(int argc, char **argv);

/// @brief set the export attribute for variables
int export(int argc, char **argv);

/// @brief close all open file descriptors
void close_fds(void);

/// @brief saves state of global variable
void save_global(struct global *global_save);

/// @brief restores state of global variable
void restore_global(struct global *global_save);

/// @brief exit builtin function
void xexit(int argc, char **argv);

/// @brief continue builtin function
int xcontinue(int agrc, char **argv);

/// @brief break builtin function
int xbreak(int argc, char **argv);

/// @brief source builtin function
int source(int argc, char **argv);

#endif /* ! BUILTINS_H */
