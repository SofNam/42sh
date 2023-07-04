#include <stdlib.h>

#include "ast.h"
#include "xmalloc.h"

struct item_case *item_case_create(void)
{
    struct item_case *item_case = xcalloc(1, sizeof(struct item_case));
    item_case->patterns = string_vector_create();
    return item_case;
}

void item_case_destroy(void *ptr)
{
    struct item_case *item_case = ptr;
    if (item_case != NULL)
    {
        string_vector_destroy(item_case->patterns);
        compound_list_ast_destroy(item_case->compound_list);
        free(item_case);
    }
}
