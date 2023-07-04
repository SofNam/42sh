#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct case_command_ast *case_command_ast_create(void)
{
    struct case_command_ast *ast = xcalloc(1, sizeof(struct case_command_ast));
    ast->items = vector_create();
    return ast;
}

void case_command_ast_destroy(void *ptr)
{
    struct case_command_ast *ast = ptr;
    if (ast != NULL)
    {
        free(ast->word);
        for (size_t i = 0; i < ast->items->size; i++)
            item_case_destroy(ast->items->data[i]);
        vector_destroy(ast->items);
        free(ast);
    }
}
