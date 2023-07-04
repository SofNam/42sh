#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct pipeline_ast *pipeline_ast_create(void)
{
    struct pipeline_ast *ast = xcalloc(1, sizeof(struct pipeline_ast));
    ast->commands = vector_create();
    return ast;
}

void pipeline_ast_destroy(void *ptr)
{
    struct pipeline_ast *ast = ptr;
    if (ast != NULL)
    {
        for (size_t i = 0; i < ast->commands->size; i++)
            command_ast_destroy(ast->commands->data[i]);
        vector_destroy(ast->commands);
        free(ast);
    }
}
