#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

void and_or_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct and_or_ast *ast = ptr;
    if (ast)
    {
        int id = rand();
        switch (ast->type)
        {
        case OR:
            fprintf(file, "%d [fillcolor=gold, label=\"||\"]\n", id);
            break;
        case AND:
            fprintf(file, "%d [fillcolor=gold, label=\"&&\"]\n", id);
            break;
        case AND_OR_NONE:
            pipeline_ast_to_dot(file, ast->ast, parent_id);
            return;
        }
        pipeline_ast_to_dot(file, ast->ast, id);
        fprintf(file, "%d -- %d\n", parent_id, id);
        and_or_ast_to_dot(file, ast->next, id);
    }
}
