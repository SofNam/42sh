#include <stdlib.h>

#include "ast.h"
#include "redir.h"
#include "string_vector.h"
#include "vector.h"
#include "xmalloc.h"

struct simple_command_ast *simple_command_ast_create(void)
{
    struct simple_command_ast *ast =
        xcalloc(1, sizeof(struct simple_command_ast));
    ast->argv = string_vector_create();
    ast->quote_type = vector_create();
    ast->redirs = vector_create();
    return ast;
}

void simple_command_ast_destroy(void *ptr)
{
    struct simple_command_ast *ast = ptr;
    if (ast != NULL)
    {
        string_vector_destroy(ast->argv);
        for (size_t i = 0; i < ast->redirs->size; i++)
            redir_destroy(ast->redirs->data[i]);
        for (size_t i = 0; i < ast->quote_type->size; i++)
            free(ast->quote_type->data[i]);
        vector_destroy(ast->redirs);
        vector_destroy(ast->quote_type);
        free(ast);
    }
}
