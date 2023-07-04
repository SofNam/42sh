#include <stdio.h>
#include <stdlib.h>

#include "ast_to_dot.h"
#include "strstrip.h"

void for_command_ast_to_dot(FILE *file, void *ptr, int parent_id)
{
    struct for_command_ast *ast = ptr;
    int id = rand();
    fprintf(file, "%d -- %d\n", parent_id, id);
    char *word = strstrip(ast->word, '\"');
    fprintf(file, "%d [fillcolor=lightblue, label=\"for %s in {", id, word);
    free(word);
    for (struct elements *e = ast->elements; e != NULL; e = e->next)
    {
        word = strstrip(e->word, '\"');
        fprintf(file, "%s", e->word);
        if (e->next != NULL)
            fprintf(file, "|");
        free(word);
    }
    fprintf(file, "}\"]\n");
    compound_list_to_dot(file, ast->body, id);
}
