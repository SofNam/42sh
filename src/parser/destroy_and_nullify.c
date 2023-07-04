#include "destroy_and_nullify.h"

#include "ast.h"

typedef void (*ast_destroy)(void *);

static const ast_destroy ast_destroy_functions[] = {
    [AST_TYPE_LIST] = list_ast_destroy,
    [AST_TYPE_AND_OR] = and_or_ast_destroy,
    [AST_TYPE_PIPELINE] = pipeline_ast_destroy,
    [AST_TYPE_SHELL] = shell_command_ast_destroy,
    [AST_TYPE_SIMPLE] = simple_command_ast_destroy,
    [AST_TYPE_COMMAND] = command_ast_destroy,
    [AST_TYPE_COMPOUND] = compound_list_ast_destroy,
    [AST_TYPE_FOR] = for_command_ast_destroy,
    [AST_TYPE_IF] = if_command_ast_destroy,
    [AST_TYPE_WHILE] = while_command_ast_destroy,
    [AST_TYPE_UNTIL] = until_command_ast_destroy,
    [AST_TYPE_CASE] = case_command_ast_destroy,
    [AST_TYPE_CASE_ITEM] = item_case_destroy
};

void *destroy_and_nullify(void *ptr, enum ast_type ast_type)
{
    ast_destroy_functions[ast_type](ptr);
    return NULL;
}
