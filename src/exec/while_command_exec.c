#include <stdlib.h>

#include "builtins.h"
#include "exec.h"
#include "loop.h"

int while_command_exec(void *ptr)
{
    struct while_command_ast *ast = ptr;
    int err = 0;
    while (compound_list_exec(ast->condition) == 0)
    {
        enum action action = loop(&err, ast->body);
        if (action == ACTION_BREAK)
            break;
        if (action == ACTION_CONTINUE)
            continue;
    }
    return err;
}
