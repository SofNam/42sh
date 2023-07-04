#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct compound_list_ast *compound_list_ast_create(void)
{
    return xcalloc(1, sizeof(struct compound_list_ast));
}

void compound_list_ast_destroy(void *ptr)
{
    struct compound_list_ast *ast = ptr;
    if (ast != NULL)
    {
        compound_list_ast_destroy(ast->next);
        and_or_ast_destroy(ast->ast);
        free(ast);
    }
}
