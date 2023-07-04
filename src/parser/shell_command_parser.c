#include <string.h>

#include "destroy_and_nullify.h"
#include "parser.h"
#include "redir.h"
#include "token.h"

typedef void *(*shell_command_parser)(struct queue *, struct hash_map *);

static const shell_command_parser shell_command_parsers[] = {
    [IF_COMMAND] = if_command_parse,
    [WHILE_COMMAND] = while_command_parse,
    [UNTIL_COMMAND] = until_command_parse,
    [FOR_COMMAND] = for_command_parse,
    [CASE_COMMAND] = case_command_parse
};

static int get_shell_command_type(char *str)
{
    if (strcmp(str, "if") == 0)
        return IF_COMMAND;
    if (strcmp(str, "while") == 0)
        return WHILE_COMMAND;
    if (strcmp(str, "until") == 0)
        return UNTIL_COMMAND;
    if (strcmp(str, "for") == 0)
        return FOR_COMMAND;
    if (strcmp(str, "case") == 0)
        return CASE_COMMAND;
    return SHELL_COMMAND_NONE;
}

static struct shell_command_ast *which_ast(struct queue *lexer,
                                           struct hash_map *variables,
                                           struct shell_command_ast *ast)
{
    struct token *token = queue_peek(lexer);
    if ((ast->type = get_shell_command_type(token->str)) == SHELL_COMMAND_NONE)
        return destroy_and_nullify(ast, AST_TYPE_SHELL);
    ast->ast = shell_command_parsers[ast->type](lexer, variables);
    return ast->ast == NULL ? destroy_and_nullify(ast, AST_TYPE_SHELL) : ast;
}

static struct shell_command_ast *compound_parse(struct queue *lexer,
                                                struct hash_map *variables,
                                                struct shell_command_ast *ast)
{
    struct token *token = queue_peek(lexer);
    if (strcmp(token->str, "{") == 0 || strcmp(token->str, "(") == 0)
    {
        queue_dequeue(lexer);
        token_free_str_and_destroy(token);
        ast->ast = compound_list_parse(lexer, variables);
        token = queue_peek(lexer);
        if (token->type == TOKEN_OPERATOR)
        {
            if (ast->ast != NULL
                && (strcmp(token->str, "}") == 0
                    || strcmp(token->str, ")") == 0))
            {
                ast->type = COMPOUND_LIST;
                queue_dequeue(lexer);
                token_free_str_and_destroy(token);
                return ast;
            }
        }
    }
    return destroy_and_nullify(ast, AST_TYPE_SHELL);
}

void *shell_command_parse(struct queue *lexer, struct hash_map *variables)
{
    struct shell_command_ast *ast = shell_command_ast_create();
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_OPERATOR)
        return compound_parse(lexer, variables, ast);
    else if (token->type == TOKEN_GENERIC)
        return which_ast(lexer, variables, ast);
    return destroy_and_nullify(ast, AST_TYPE_SHELL);
}
