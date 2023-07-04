#ifndef AST_H
#define AST_H

#include <stdbool.h>

#include "string_vector.h"
#include "vector.h"

/// @file ast.h
/// @brief Definition of all the ASTs.

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent a for statement
struct and_or_ast
{
    enum
    {
        AND_OR_NONE,
        AND,
        OR
    } type;
    struct pipeline_ast *ast;
    struct and_or_ast *next;
};

/// Create an empty and_or_ast.
struct and_or_ast *and_or_ast_create(void);

/// Deallocate the space allocated to the given and_or_ast.
void and_or_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent a for statement
struct case_command_ast
{
    char *word;
    struct vector *items;
};

/// Creates an empty case_command_ast.
struct case_command_ast *case_command_ast_create(void);

/// Deallocates the space allocated to the given case_command_ast.
void case_command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent the command rule.
struct command_ast
{
    enum
    {
        NO_AST,
        SIMPLE_COMMAND_AST,
        SHELL_COMMAND_AST,
    } type;
    void *ast;
};

/// Create an empty command_ast of type NO_AST.
struct command_ast *command_ast_create(void);

/// Deallocate the space allocated to the given command_ast.
void command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent the compound list rule.
struct compound_list_ast
{
    struct and_or_ast *ast;
    struct compound_list_ast *next;
};

/// Create an empty compound_list_ast.
struct compound_list_ast *compound_list_ast_create(void);

/// Deallocate the space allocated to the given compound_list_ast.
void compound_list_ast_destroy(void *ptr);

struct elements
{
    char *word;
    struct elements *next;
};

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent a for statement
struct for_command_ast
{
    char *word;
    struct elements *elements;
    struct compound_list_ast *body;
};

/// Creates an empty for_command_ast.
struct for_command_ast *for_command_ast_create(void);

/// Deallocates the space allocated to the given for_command_ast.
void for_command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent an if statement.
struct if_command_ast
{
    enum
    {
        IF_COMMAND_NONE,
        ELIF,
        ELSE
    } type;
    struct compound_list_ast *condition;
    struct compound_list_ast *if_body;
    void *else_body;
};

/// Create an empty if_command_ast of type NONE.
struct if_command_ast *if_command_ast_create(void);

/// Deallocate the space allocated to the given if_command_ast.
void if_command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent case item rule
struct item_case
{
    struct string_vector *patterns;
    struct compound_list_ast *compound_list;
};

/// Creates an empty item_case.
struct item_case *item_case_create(void);

/// Deallocates the space allocated to the given item_case.
void item_case_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent the list rule
struct list_ast
{
    struct and_or_ast *ast;
    struct list_ast *next;
};

/// Create an empty list_ast.
struct list_ast *list_ast_create(void);

/// Deallocate the space allocated to the given list_ast.
void list_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent the pipeline rule
struct pipeline_ast
{
    bool negate_return;
    struct vector *commands;
};

/// Create an empty pipeline_ast.
struct pipeline_ast *pipeline_ast_create(void);

/// Deallocate the space allocated to the given pipeline_ast.
void pipeline_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// @brief Struct for shell commands.
struct shell_command_ast
{
    enum
    {
        SHELL_COMMAND_NONE = 0,
        IF_COMMAND,
        WHILE_COMMAND,
        UNTIL_COMMAND,
        FOR_COMMAND,
        CASE_COMMAND,
        COMPOUND_LIST
    } type;
    void *ast;
    struct vector *redirs;
};

/// Create an empty shell_command_ast.
struct shell_command_ast *shell_command_ast_create(void);

/// Deallocate the space allocated to the given shell_command_ast.
void shell_command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent a simple command.
struct simple_command_ast
{
    struct string_vector *argv;
    struct vector *quote_type;
    struct vector *redirs;
};

/// Create an empty simple_command_ast.
struct simple_command_ast *simple_command_ast_create(void);

/// Deallocate the space allocated to the given simple_command_ast.
void simple_command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent a until statement
struct until_command_ast
{
    struct compound_list_ast *condition;
    struct compound_list_ast *body;
};

/// Creates an empty until_command_ast.
struct until_command_ast *until_command_ast_create(void);

/// Deallocates the space allocated to the given until_command_ast.
void until_command_ast_destroy(void *ptr);

////////////////////////////////////////////////////////////////////////////////
/// AST used to represent a while statement
struct while_command_ast
{
    struct compound_list_ast *condition;
    struct compound_list_ast *body;
};

/// Creates an empty while_command_ast.
struct while_command_ast *while_command_ast_create(void);

/// Deallocates the space allocated to the given while_command_ast.
void while_command_ast_destroy(void *ptr);

#endif /* ! AST_H */
