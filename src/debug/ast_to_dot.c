#include "ast_to_dot.h"

#include <unistd.h>
#include <xio.h>

static FILE *create_or_truncate_dot_file(void)
{
    return xfopen("ast.dot", "w");
}

static FILE *open_and_append_dot_file(void)
{
    return xfopen("ast.dot", "a");
}

static void write_dot_file_content(FILE *file, struct list_ast *ast)
{
    fprintf(file, "graph {\n");
    fprintf(file,
            "node [shape=box, style=\"rounded,filled\", "
            "fillcolor=bisque]\n");
    list_ast_to_dot(file, ast);
    fprintf(file, "}\n");
}

void ast_to_dot(struct list_ast *ast)
{
    FILE *file = NULL;
    if (access("ast.dot", R_OK) == 0)
        file = open_and_append_dot_file();
    else
        file = create_or_truncate_dot_file();
    write_dot_file_content(file, ast);
    xfclose(file);
}
