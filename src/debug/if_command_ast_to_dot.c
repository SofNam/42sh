#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

#define IS_ELIF true
#define IS_IF false

static void if_command_ast_to_dot_rec(FILE *file, struct if_command_ast *ast,
                                      int parent_id, bool is_elif)
{
    int id = rand();
    if (is_elif)
        fprintf(file, "%d [fillcolor=tomato, label=\"elif\"]\n", id);
    else
        fprintf(file, "%d [fillcolor=tomato, label=\"if\"]\n", id);
    fprintf(file, "%d -- %d\n", parent_id, id);
    compound_list_to_dot(file, ast->condition, id);
    compound_list_to_dot(file, ast->if_body, id);
    if (ast->type == ELIF)
        if_command_ast_to_dot_rec(file, ast->else_body, id, IS_ELIF);
    else if (ast->type == ELSE)
        compound_list_to_dot(file, ast->else_body, id);
}

void if_command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct if_command_ast *ast = ptr;
    if (ast)
        if_command_ast_to_dot_rec(file, ast, parent_id, IS_IF);
}
