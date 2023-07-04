#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

typedef void (*ast_destroyer)(void *);

static const ast_destroyer ast_destroyers[] = {
    [ELIF] = if_command_ast_destroy,
    [ELSE] = compound_list_ast_destroy,
};

struct if_command_ast *if_command_ast_create(void)
{
    return xcalloc(1, sizeof(struct if_command_ast));
}

void if_command_ast_destroy(void *ptr)
{
    struct if_command_ast *ast = ptr;
    if (ast != NULL)
    {
        compound_list_ast_destroy(ast->condition);
        compound_list_ast_destroy(ast->if_body);
        if (ast->type != IF_COMMAND_NONE)
            ast_destroyers[ast->type](ast->else_body);
        free(ast);
    }
}
