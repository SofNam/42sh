#include "ast_to_dot.h"

typedef void (*ast_print)(FILE *, void *, int);

static const ast_print ast_printers[] = {
    [SIMPLE_COMMAND_AST] = simple_command_ast_to_dot,
    [SHELL_COMMAND_AST] = shell_command_ast_to_dot,
};

void command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct command_ast *ast = ptr;
    if (ast)
        ast_printers[ast->type](file, ast->ast, parent_id);
}
