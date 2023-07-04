#include "builtins.h"
#include "exec.h"

extern struct global global;

int list_exec(struct list_ast *ast)
{
    global.cont = 0;
    global.nested_loop = 0;
    int err = and_or_exec(ast->ast);
    if (ast->next != NULL)
        err = list_exec(ast->next);
    return err;
}
