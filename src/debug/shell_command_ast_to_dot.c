#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"

typedef void (*ast_print)(FILE *, void *, int);

static const ast_print ast_printers[] = {
    [IF_COMMAND] = if_command_ast_to_dot,
    [WHILE_COMMAND] = while_command_ast_to_dot,
    [UNTIL_COMMAND] = until_command_ast_to_dot,
    [FOR_COMMAND] = for_command_ast_to_dot,
    [CASE_COMMAND] = case_command_ast_to_dot,
    [COMPOUND_LIST] = compound_list_to_dot,
};

void shell_command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct shell_command_ast *ast = ptr;
    ast_printers[ast->type](file, ast->ast, parent_id);
}
