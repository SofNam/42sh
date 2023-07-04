#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct and_or_ast *and_or_ast_create(void)
{
    return xcalloc(1, sizeof(struct and_or_ast));
}

void and_or_ast_destroy(void *ptr)
{
    struct and_or_ast *ast = ptr;
    if (ast != NULL)
    {
        and_or_ast_destroy(ast->next);
        pipeline_ast_destroy(ast->ast);
        free(ast);
    }
}
