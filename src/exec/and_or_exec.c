#include <stdbool.h>

#include "builtins.h"
#include "exec.h"

static int and_or_exec_rec(void *ptr, bool skip)
{
    struct and_or_ast *ast = ptr;
    int err = skip ? 0 : pipeline_exec(ast->ast);
    if (ast->type == AND && err == 0)
        return and_or_exec_rec(ast->next, false);
    if (ast->type == OR)
        return and_or_exec_rec(ast->next, err == 0);
    return err;
}

int and_or_exec(void *ptr)
{
    struct and_or_ast *ast = ptr;
    return and_or_exec_rec(ast, false);
}
