#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtins.h"
#include "token.h"
#include "xunistd.h"

extern struct global global;

void close_fds(void)
{
    for (size_t i = 0; i < global.open_fds; i++)
        xclose(global.fds[i]);
    for (size_t i = 0; i < global.open_redir_fds; i++)
        xclose(global.redir_fds[i]);
}

void xexit(int argc, char **argv)
{
    close_fds();
    int status = argc == 2 ? atoi(argv[1]) : global.last_return;
    list_ast_destroy(global.ast);
    hash_map_destroy(global.variables);
    destroy_remaining_tokens(global.lexer);
    queue_destroy(global.lexer);
    exit(status);
    warnx("exit: too many arguments");
}
