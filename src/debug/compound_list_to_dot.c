#include "ast_to_dot.h"

void compound_list_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct compound_list_ast *ast = ptr;
    if (ast)
    {
        and_or_ast_to_dot(file, ast->ast, parent_id);
        compound_list_to_dot(file, ast->next, parent_id);
    }
}
