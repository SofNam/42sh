#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

void list_ast_to_dot(FILE *file, struct list_ast *ast)
{
    if (ast)
    {
        int id = rand();
        fprintf(file, "%d [fillcolor=darksalmon, label=\"list\"]\n", id);
        and_or_ast_to_dot(file, ast->ast, id);
    }
}
