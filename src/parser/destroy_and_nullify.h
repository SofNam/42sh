#ifndef DESTROY_AND_NULLIFY_H
#define DESTROY_AND_NULLIFY_H

/// @file destroy_and_nullify.h
/// @brief Definition of a function to destroy an ast and return NULL.

/// @brief enum of different ast types
enum ast_type
{
    AST_TYPE_LIST,
    AST_TYPE_AND_OR,
    AST_TYPE_PIPELINE,
    AST_TYPE_SHELL,
    AST_TYPE_SIMPLE,
    AST_TYPE_COMMAND,
    AST_TYPE_COMPOUND,
    AST_TYPE_FOR,
    AST_TYPE_IF,
    AST_TYPE_WHILE,
    AST_TYPE_UNTIL,
    AST_TYPE_CASE,
    AST_TYPE_CASE_ITEM
};

/// @brief destroy the ast with given type
void *destroy_and_nullify(void *ptr, enum ast_type ast_type);

#endif /* ! DESTROY_AND_NULLIFY_H */
