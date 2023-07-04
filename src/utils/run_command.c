#include "run_command.h"

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "xio.h"
#include "xunistd.h"

extern char **environ;
extern struct global global;

static int get_child_status(int pid)
{
    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        err(1, "failed to wait for child");
    return WEXITSTATUS(wstatus);
}

static void close_pipe_fds(void)
{
    int count = global.open_fds;
    if (count == 1)
    {
        xclose(global.fds[0]);
    }
    else if (count == 2)
    {
        xclose(global.fds[0]);
        xclose(global.fds[1]);
    }
    global.open_fds = 0;
}

static int run_if_is_builtin(int argc, char **argv)
{
    if (strcmp(argv[0], "pwd") == 0)
        return pwd();
    if (strcmp(argv[0], "cd") == 0)
        return cd(argc, argv);
    if (strcmp(argv[0], "echo") == 0)
        return echo(argc, argv);
    if (strcmp(argv[0], "export") == 0)
        return export(argc, argv);
    if (strcmp(argv[0], "continue") == 0)
        return xcontinue(argc, argv);
    if (strcmp(argv[0], "break") == 0)
        return xbreak(argc, argv);
    if (strcmp(argv[0], "source") == 0 || strcmp(argv[0], ".") == 0)
        return source(argc, argv);
    if (strcmp(argv[0], "alias") == 0)
        return alias(argc, argv);
    if (strcmp(argv[0], "unalias") == 0)
        return unalias(argc, argv);
    if (strcmp(argv[0], "exit") == 0)
        xexit(argc, argv);
    return -1;
}

int run_command(int argc, char **argv)
{
    int err = run_if_is_builtin(argc, argv);
    if (err != -1)
    {
        fflush(stdout);
        return err;
    }
    int pid = xfork();
    if (pid == 0)
    {
        close_pipe_fds();
        if (execvp(argv[0], argv) == -1)
        {
            fprintf(stderr, "42sh: %s: command not found\n", argv[0]);
            exit(127);
        }
    }
    return get_child_status(pid);
}
