#include "exec.h"

int command_exec(void *ptr)
{
    struct command_ast *ast = ptr;
    if (ast->type == SIMPLE_COMMAND_AST)
        return simple_command_exec(ast->ast);
    if (ast->type == SHELL_COMMAND_AST)
        return shell_command_exec(ast->ast);
    return 0;
}
