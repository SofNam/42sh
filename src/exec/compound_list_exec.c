#include "builtins.h"
#include "exec.h"

extern struct global global;

int compound_list_exec(void *ptr)
{
    struct compound_list_ast *ast = ptr;
    int err = and_or_exec(ast->ast);
    if (global.cont <= 0 && global.breaky <= 0)
    {
        if (ast->next != NULL)
            err = compound_list_exec(ast->next);
    }
    return err;
}
