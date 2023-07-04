#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

typedef void (*ast_destroyer)(void *);

static const ast_destroyer ast_destroyers[] = {
    [SIMPLE_COMMAND_AST] = simple_command_ast_destroy,
    [SHELL_COMMAND_AST] = shell_command_ast_destroy,
};

struct command_ast *command_ast_create(void)
{
    return xcalloc(1, sizeof(struct command_ast));
}

void command_ast_destroy(void *ptr)
{
    struct command_ast *ast = ptr;
    if (ast != NULL)
    {
        ast_destroyers[ast->type](ast->ast);
        free(ast);
    }
}
