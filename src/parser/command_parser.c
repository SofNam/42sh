#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "destroy_and_nullify.h"
#include "parser.h"
#include "redir.h"
#include "token.h"

static bool is_shell_command(struct token *token)
{
    return (token->type == TOKEN_GENERIC
            && (strcmp(token->str, "for") == 0
                || strcmp(token->str, "while") == 0
                || strcmp(token->str, "until") == 0
                || strcmp(token->str, "if") == 0
                || strcmp(token->str, "case") == 0))
        || (token->type == TOKEN_OPERATOR
            && (strcmp(token->str, "(") == 0 || strcmp(token->str, "{") == 0));
}

static bool is_number(const char *string)
{
    size_t i = 0;
    while (string[i] != 0)
    {
        if (string[i] < '0' || string[i] > '9')
            return false;
        i++;
    }
    return true;
}

static bool parse_redir(struct queue *lexer, struct shell_command_ast *ast)
{
    struct redir *redir = redir_create();
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_IONUMBER)
    {
        redir->io = atoi(token->str);
        token = get_next_token(lexer);
    }
    if (token->type == TOKEN_REDIROP)
    {
        redir->type = which_redir_operator(token->str);
        redir->io = redir->io == -1 ? io_from_redirop(redir->type) : redir->io;
    }
    else
    {
        redir_destroy(redir);
        return false;
    }
    token = get_next_token(lexer);
    if (token->type == TOKEN_GENERIC)
    {
        if (redir->type == LESSAND && !is_number(token->str))
        {
            redir_destroy(redir);
            return false;
        }
        redir->word = token->str;
        queue_dequeue(lexer);
        token_destroy(token);
        vector_add(ast->redirs, redir);
        return true;
    }
    redir_destroy(redir);
    return false;
}

void *command_parse(struct queue *lexer, struct hash_map *variables)
{
    struct command_ast *ast = command_ast_create();
    if (is_shell_command(queue_peek(lexer)))
    {
        ast->type = SHELL_COMMAND_AST;
        ast->ast = shell_command_parse(lexer, variables);
        if (ast->ast == NULL)
            return destroy_and_nullify(ast, AST_TYPE_COMMAND);
        struct token *token = queue_peek(lexer);
        while (token->type == TOKEN_IONUMBER || token->type == TOKEN_REDIROP)
        {
            bool err = parse_redir(lexer, ast->ast);
            if (!err)
                return destroy_and_nullify(ast, AST_TYPE_COMMAND);
            token = queue_peek(lexer);
        }
    }
    else
    {
        ast->type = SIMPLE_COMMAND_AST;
        ast->ast = simple_command_parse(lexer, variables);
        if (ast->ast == NULL)
            return destroy_and_nullify(ast, AST_TYPE_COMMAND);
    }
    return ast;
}
