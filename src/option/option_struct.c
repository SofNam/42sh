#include <stdlib.h>

#include "option.h"
#include "xmalloc.h"

struct option *option_init(void)
{
    struct option *option = xcalloc(1, sizeof(struct option));
    option->c = xcalloc(1, sizeof(struct option_c));
    option->O = xcalloc(1, sizeof(struct option_O));
    option->filename = xcalloc(1, sizeof(struct option_filename));
    option->gnu = xcalloc(1, sizeof(struct gnu_option));
    option->gnu->e_gnu = NO_OPTION;
    return option;
}

struct option_O *get_last_O(struct option *option)
{
    if (option == NULL)
        return NULL;
    if (option->O->state == 0)
        return option->O;
    struct option_O *new = xcalloc(1, sizeof(struct option_O));
    struct option_O *current = option->O;
    while (current->next)
        current = current->next;
    current->next = new;
    return new;
}

void free_option(struct option *option)
{
    if (option->error)
        free(option->error);
    struct option_O *current = option->O;
    while (current)
    {
        struct option_O *delete = current;
        current = current->next;
        free(delete);
    }
    free(option->c);
    free(option->gnu);
    free(option->filename);
    free(option);
}
