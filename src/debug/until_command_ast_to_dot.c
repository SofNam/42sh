#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

void until_command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct until_command_ast *ast = ptr;
    int id = rand();
    fprintf(file, "%d [fillcolor=violet, label=\"until\"]\n", id);
    fprintf(file, "%d -- %d\n", parent_id, id);
    compound_list_to_dot(file, ast->condition, id);
    compound_list_to_dot(file, ast->body, id);
}
