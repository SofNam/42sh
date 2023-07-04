#include <stdlib.h>

#include "ast.h"
#include "redir.h"
#include "xmalloc.h"

typedef void (*ast_destroyer)(void *);

static const ast_destroyer ast_destroyers[] = {
    [IF_COMMAND] = if_command_ast_destroy,
    [WHILE_COMMAND] = while_command_ast_destroy,
    [UNTIL_COMMAND] = until_command_ast_destroy,
    [FOR_COMMAND] = for_command_ast_destroy,
    [CASE_COMMAND] = case_command_ast_destroy,
    [COMPOUND_LIST] = compound_list_ast_destroy
};

struct shell_command_ast *shell_command_ast_create(void)
{
    struct shell_command_ast *ast =
        xcalloc(1, sizeof(struct shell_command_ast));
    ast->redirs = vector_create();
    return ast;
}

void shell_command_ast_destroy(void *ptr)
{
    struct shell_command_ast *ast = ptr;
    if (ast != NULL)
    {
        for (size_t i = 0; i < ast->redirs->size; i++)
            redir_destroy(ast->redirs->data[i]);
        vector_destroy(ast->redirs);
        ast_destroyers[ast->type](ast->ast);
        free(ast);
    }
}
