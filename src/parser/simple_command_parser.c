#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "destroy_and_nullify.h"
#include "expansion.h"
#include "hash_map.h"
#include "is_number.h"
#include "name.h"
#include "redir.h"
#include "token.h"
#include "xmalloc.h"
#include "xstring.h"

static void split_assignment(char *str, char *equal_sign_ptr, char **var,
                             char **value)
{
    size_t len = xstrlen(str);
    size_t var_len = equal_sign_ptr - str;
    *var = xstrndup(str, var_len);
    *value = xstrndup(equal_sign_ptr + 1, len - 1 - var_len);
    free(str);
}

static bool parse_redir(struct queue *lexer, struct simple_command_ast *ast)
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

static bool parse_prefix(struct queue *lexer, struct hash_map *variables,
                         struct simple_command_ast *ast)
{
    struct token *token = queue_peek(lexer);
    char *equal_sign_ptr = NULL;
    char *var = NULL;
    char *value = NULL;
    while ((token->type == TOKEN_GENERIC
            && (equal_sign_ptr = strchr(token->str, '=')) != NULL)
           || token->type == TOKEN_IONUMBER || token->type == TOKEN_REDIROP)
    {
        if (token->type == TOKEN_GENERIC)
        {
            if (equal_sign_ptr == token->str
                || !is_valid_name(token->str, equal_sign_ptr))
                break;
            queue_dequeue(lexer);
            split_assignment(token->str, equal_sign_ptr, &var, &value);
            hash_map_insert(variables, var, value);
            token_destroy(token);
        }
        else
        {
            if (!parse_redir(lexer, ast))
                return false;
        }
        token = queue_peek(lexer);
    }
    return true;
}

static struct simple_command_ast *parse_elements(struct queue *lexer,
                                                 struct simple_command_ast *ast)
{
    struct token *token = queue_peek(lexer);
    while (token->type == TOKEN_GENERIC || token->type == TOKEN_IONUMBER
           || token->type == TOKEN_REDIROP
           || (token->type == TOKEN_OPERATOR && strcmp(token->str, ")") != 0
               && strcmp(token->str, "}") != 0))
    {
        if (token->type == TOKEN_GENERIC)
        {
            token = queue_dequeue(lexer);
            tilde_expansion(token);
            string_vector_add(ast->argv, token->str);
            int *quote_type = xcalloc(1, sizeof(int));
            *quote_type = token->quote;
            vector_add(ast->quote_type, quote_type);
            token_destroy(token);
        }
        else if (token->type != TOKEN_OPERATOR)
        {
            if (!parse_redir(lexer, ast))
                return destroy_and_nullify(ast, AST_TYPE_SIMPLE);
        }
        else
        {
            break;
        }
        token = queue_peek(lexer);
    }
    return ast;
}

struct simple_command_ast *simple_command_parse(struct queue *lexer,
                                                struct hash_map *variables)
{
    struct token *token = queue_peek(lexer);
    struct simple_command_ast *ast = simple_command_ast_create();
    if (token->type == TOKEN_OPERATOR || token->type == TOKEN_EOF)
        return destroy_and_nullify(ast, AST_TYPE_SIMPLE);
    if (!parse_prefix(lexer, variables, ast))
        return destroy_and_nullify(ast, AST_TYPE_SIMPLE);
    return parse_elements(lexer, ast);
}
