#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"
#include "strstrip.h"

static void item_case_to_dot(FILE *file, struct item_case *item, int parent_id)
{
    int id = rand();
    size_t last_pattern = item->patterns->size - 1;
    char *pattern = NULL;
    fprintf(file, "%d -- %d\n", parent_id, id);
    fprintf(file, "%d [fillcolor=aquamarine, label=\"in {", id);
    for (size_t i = 0; i < last_pattern; ++i)
    {
        pattern = strstrip(item->patterns->strings[i], '\"');
        fprintf(file, "%s|", pattern);
        free(pattern);
    }
    pattern = strstrip(item->patterns->strings[last_pattern], '\"');
    fprintf(file, "%s}\"]\n", pattern);
    free(pattern);
    compound_list_to_dot(file, item->compound_list, id);
}

void case_command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct case_command_ast *ast = ptr;
    int id = rand();
    char *word = strstrip(ast->word, '\"');
    fprintf(file, "%d -- %d\n", parent_id, id);
    fprintf(file, "%d [fillcolor=lawngreen, label=\"case %s\"]\n", id, word);
    free(word);
    for (size_t i = 0; i < ast->items->size; ++i)
        item_case_to_dot(file, ast->items->data[i], id);
}
