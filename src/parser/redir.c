#include "redir.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xmalloc.h"

struct redir *redir_create(void)
{
    struct redir *redir = xcalloc(1, sizeof(struct redir));
    redir->io = -1;
    redir->io_save = -1;
    return redir;
}

void redir_destroy(struct redir *redir)
{
    if (redir != NULL)
    {
        free(redir->word);
        free(redir);
    }
}

int which_redir_operator(const char *operator)
{
    if (strcmp(operator, ">") == 0)
        return GREAT;
    if (strcmp(operator, ">>") == 0)
        return DGREAT;
    if (strcmp(operator, "<") == 0)
        return LESS;
    if (strcmp(operator, "<<") == 0)
        return DLESS;
    if (strcmp(operator, "<&") == 0)
        return LESSAND;
    if (strcmp(operator, ">&") == 0)
        return GREATAND;
    if (strcmp(operator, "<>") == 0)
        return LESSGREAT;
    if (strcmp(operator, "<<-") == 0)
        return DLESSDASH;
    return CLOBBER; // >|
}

int io_from_redirop(int redirop)
{
    switch (redirop)
    {
    case GREAT:
    case DGREAT:
    case GREATAND:
    case CLOBBER:
        return STDOUT_FILENO;
    case LESS:
    case DLESS:
    case DLESSDASH:
    case LESSAND:
        return STDIN_FILENO;
    default:
        return -1;
    }
}
