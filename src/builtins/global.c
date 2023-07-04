#include <stddef.h>

#include "builtins.h"

extern struct global global;

void save_global(struct global *global_save)
{
    global_save->last_return = global.last_return;
    global_save->open_fds = global.open_fds;
    for (size_t i = 0; i < 2; i++)
        global_save->fds[i] = global.fds[i];
    global_save->open_redir_fds = global.open_redir_fds;
    for (size_t i = 0; i < 3; i++)
        global_save->redir_fds[i] = global.redir_fds[i];
    global_save->ast = global.ast;
    global_save->lexer = global.lexer;
    global_save->variables = global.variables;
    global_save->aliases = global.aliases;
    global_save->cont = global.cont;
    global_save->nested_loop = global.nested_loop;
    global_save->continue_err = global.continue_err;
    global_save->breaky = global.breaky;
    global_save->breaky_err = global.breaky_err;
}

void restore_global(struct global *global_save)
{
    close_fds();
    hash_map_destroy(global.variables);
    hash_map_destroy(global.aliases);
    global.open_fds = global_save->open_fds;
    for (size_t i = 0; i < 2; i++)
        global.fds[i] = global_save->fds[i];
    global.open_redir_fds = global_save->open_redir_fds;
    for (size_t i = 0; i < 3; i++)
        global.redir_fds[i] = global_save->redir_fds[i];
    global.ast = global_save->ast;
    global.lexer = global_save->lexer;
    global.variables = global_save->variables;
    global.aliases = global_save->aliases;
    global.cont = global_save->cont;
    global.nested_loop = global_save->nested_loop;
    global.continue_err = global_save->continue_err;
    global.breaky = global_save->breaky;
    global.breaky_err = global_save->breaky_err;
}
