#include "loop.h"

#include "builtins.h"
#include "exec.h"

extern struct global global;

static void increment_global_var(void)
{
    global.nested_loop--;
    global.cont--;
    global.breaky--;
}

enum action loop(int *err, struct compound_list_ast *body)
{
    global.nested_loop++;
    *err = compound_list_exec(body);
    increment_global_var();
    if (global.nested_loop == 0)
    {
        if (global.cont >= 0)
        {
            global.cont = 0;
            return ACTION_CONTINUE;
        }
        if (global.breaky >= 0)
        {
            global.breaky = 0;
            return ACTION_BREAK;
        }
    }
    else if (global.continue_err == 0)
    {
        if (global.cont == 0)
        {
            global.nested_loop = 0;
            return ACTION_CONTINUE;
        }
        if (global.breaky == 0)
        {
            global.nested_loop = 0;
            return ACTION_BREAK;
        }
    }
    if (((global.cont > 0 && global.nested_loop > 0)
         || global.continue_err == -1)
        || (global.breaky > 0 && global.nested_loop > 0)
        || global.breaky_err == -1)
        return ACTION_BREAK;
    return ACTION_NONE;
}
