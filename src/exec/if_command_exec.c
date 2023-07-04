#include <stdlib.h>

#include "exec.h"

int if_command_exec(void *ptr)
{
    struct if_command_ast *ast = ptr;
    if (compound_list_exec(ast->condition) == 0)
        return compound_list_exec(ast->if_body);
    if (ast->else_body == NULL)
        return 0;
    if (ast->type == ELIF)
        return if_command_exec(ast->else_body);
    return compound_list_exec(ast->else_body);
}
