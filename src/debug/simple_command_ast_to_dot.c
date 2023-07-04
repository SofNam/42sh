#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

void simple_command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct simple_command_ast *ast = ptr;
    if (ast)
    {
        int id = rand();
        fprintf(file, "%d -- %d\n", parent_id, id);
        fprintf(file, "%d [fillcolor=orange, label=\"", id);
        for (size_t i = 0; i < ast->argv->size; i++)
            fprintf(file, "%s ", ast->argv->strings[i]);
        fprintf(file, "\"]\n");
    }
}
