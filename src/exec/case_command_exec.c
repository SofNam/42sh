#include <stdbool.h>

#include "builtins.h"
#include "exec.h"
#include "xfnmatch.h"

int case_command_exec(void *ptr)
{
    struct case_command_ast *ast = ptr;
    struct item_case *item_case = NULL;
    bool match = false;
    for (size_t i = 0; i < ast->items->size; i++)
    {
        item_case = ast->items->data[i];
        for (size_t p = 0; p < item_case->patterns->size; p++)
        {
            if (xfnmatch(item_case->patterns->strings[p], ast->word, 0))
            {
                match = true;
                break;
            }
        }
        if (match && item_case->compound_list != NULL)
            return compound_list_exec(item_case->compound_list);
    }
    return 0;
}
