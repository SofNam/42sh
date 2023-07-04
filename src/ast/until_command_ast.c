#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct until_command_ast *until_command_ast_create(void)
{
    return xcalloc(1, sizeof(struct until_command_ast));
}

void until_command_ast_destroy(void *ptr)
{
    struct until_command_ast *ast = ptr;
    if (ast != NULL)
    {
        compound_list_ast_destroy(ast->condition);
        compound_list_ast_destroy(ast->body);
        free(ast);
    }
}
