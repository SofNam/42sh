#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct list_ast *list_ast_create(void)
{
    return xcalloc(1, sizeof(struct list_ast));
}

void list_ast_destroy(void *ptr)
{
    struct list_ast *ast = ptr;
    if (ast != NULL)
    {
        list_ast_destroy(ast->next);
        and_or_ast_destroy(ast->ast);
        free(ast);
    }
}
