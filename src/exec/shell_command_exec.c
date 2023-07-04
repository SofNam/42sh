#include <stdlib.h>

#include "exec.h"

typedef int (*ast_exec)(void *);

static const ast_exec ast_exec_functions[] = {
    [IF_COMMAND] = if_command_exec,       [WHILE_COMMAND] = while_command_exec,
    [UNTIL_COMMAND] = until_command_exec, [FOR_COMMAND] = for_command_exec,
    [CASE_COMMAND] = case_command_exec,   [COMPOUND_LIST] = compound_list_exec
};

int shell_command_exec(void *ptr)
{
    struct shell_command_ast *ast = ptr;
    int *io_save_arr = NULL;
    int largest_io = get_largest_io(ast->redirs, &io_save_arr);
    int return_value = 0;
    setup_redirections(ast->redirs, io_save_arr);
    return_value = ast_exec_functions[ast->type](ast->ast);
    unredirect(ast->redirs, io_save_arr, largest_io);
    free(io_save_arr);
    return return_value;
}
