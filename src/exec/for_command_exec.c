#include <stdlib.h>

#include "builtins.h"
#include "exec.h"
#include "hash_map.h"
#include "loop.h"
#include "xstring.h"

extern struct global global;

int for_command_exec(void *ptr)
{
    struct for_command_ast *ast = ptr;
    int err = 0;
    for (struct elements *curr = ast->elements; curr != NULL; curr = curr->next)
    {
        hash_map_insert(global.variables, xstrdup(ast->word),
                        xstrdup(curr->word));
        enum action action = loop(&err, ast->body);
        if (action == ACTION_BREAK)
            break;
        if (action == ACTION_CONTINUE)
            continue;
    }
    hash_map_remove(global.variables, ast->word);
    return err;
}
