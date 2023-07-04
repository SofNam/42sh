#include <err.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtins.h"
#include "exec.h"
#include "xio.h"
#include "xmalloc.h"
#include "xunistd.h"

extern struct global global;

static int negate_return(struct pipeline_ast *ast, int return_code)
{
    return ast->negate_return ? !return_code : return_code;
}

static int run_redirect_command(struct command_ast *command, int fd, int stream)
{
    int std_dup = xdup(stream);
    xdup2(fd, stream);
    global.open_fds = 1;
    global.fds[0] = fd;
    int err = command_exec(command);
    xdup2(std_dup, stream);
    xclose(fd);
    xclose(std_dup);
    return err;
}

static void redirect_command(int *all_fds, size_t i, int *stdin, int *stdout)
{
    global.open_fds = 2;
    *stdin = xdup(STDIN_FILENO);
    xdup2(all_fds[(i - 1) * 2], STDIN_FILENO);
    global.fds[0] = all_fds[(i - 1) * 2];
    *stdout = xdup(STDOUT_FILENO);
    xdup2(all_fds[2 * i + 1], STDOUT_FILENO);
    global.fds[1] = all_fds[2 * i + 1];
}

static void unredirect_command(int *all_fds, size_t i, int stdin, int stdout)
{
    xdup2(stdin, STDIN_FILENO);
    xclose(all_fds[(i - 1) * 2]);
    xdup2(stdout, STDOUT_FILENO);
    xclose(all_fds[2 * i + 1]);
    xclose(stdin);
    xclose(stdout);
}

static void pipeline_exec_core(struct pipeline_ast *ast, int *all_fds, size_t i)
{
    int stdin;
    int stdout;
    redirect_command(all_fds, i, &stdin, &stdout);
    command_exec(ast->commands->data[i]);
    unredirect_command(all_fds, i, stdin, stdout);
}

int pipeline_exec(void *ptr)
{
    struct pipeline_ast *ast = ptr;
    if (ast->commands->size == 1)
        return negate_return(ast, command_exec(ast->commands->data[0]));
    size_t nb_pipes = (ast->commands->size - 1) * 2;
    int *all_fds = xcalloc(nb_pipes, sizeof(int));
    for (size_t i = 0; i < nb_pipes; i += 2)
        xpipe(all_fds + i);
    run_redirect_command(ast->commands->data[0], all_fds[1], STDOUT_FILENO);
    for (size_t i = 1; i < ast->commands->size - 1; i++)
        pipeline_exec_core(ast, all_fds, i);
    int err = run_redirect_command(ast->commands->data[ast->commands->size - 1],
                                   all_fds[nb_pipes - 2], STDIN_FILENO);
    global.open_fds = 0;
    free(all_fds);
    return negate_return(ast, err);
}
