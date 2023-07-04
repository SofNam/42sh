#ifndef LOOP_H
#define LOOP_H

#include "ast.h"

/// @file loop.h
/// @brief Contains definition of loop

/// @brief enum of either break, continue, or none
enum action
{
    ACTION_NONE,
    ACTION_BREAK,
    ACTION_CONTINUE
};

/// @brief creates a loop
enum action loop(int *err, struct compound_list_ast *body);

#endif /* ! LOOP_H */
