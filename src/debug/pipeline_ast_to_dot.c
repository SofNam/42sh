#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

void pipeline_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct pipeline_ast *ast = ptr;
    if (ast)
    {
        int id = rand();
        if (ast->commands->size == 1)
        {
            command_ast_to_dot(file, ast->commands->data[0], parent_id);
        }
        else
        {
            for (size_t i = 0; i < ast->commands->size; i++)
                command_ast_to_dot(file, ast->commands->data[i], id);
            fprintf(file, "%d -- %d\n", parent_id, id);
            if (ast->negate_return)
                fprintf(file, "%d [fillcolor=yellow, label=\"! |\"]\n", id);
            else
                fprintf(file, "%d [fillcolor=yellow, label=\"|\"]\n", id);
        }
    }
}
