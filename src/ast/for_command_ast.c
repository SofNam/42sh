#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct for_command_ast *for_command_ast_create(void)
{
    return xcalloc(1, sizeof(struct for_command_ast));
}

static void elements_destroy(struct elements *elements)
{
    if (elements != NULL)
    {
        elements_destroy(elements->next);
        free(elements->word);
        free(elements);
    }
}

void for_command_ast_destroy(void *ptr)
{
    struct for_command_ast *ast = ptr;
    if (ast != NULL)
    {
        elements_destroy(ast->elements);
        compound_list_ast_destroy(ast->body);
        free(ast->word);
        free(ast);
    }
}
