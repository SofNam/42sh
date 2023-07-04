#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "destroy_and_nullify.h"
#include "name.h"
#include "next_is_keyword.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"
#include "xmalloc.h"
#include "xstring.h"

static bool get_first_word(struct queue *lexer, struct for_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    token = queue_dequeue(lexer);
    if (token->type == TOKEN_GENERIC
        && is_valid_name(token->str, token->str + xstrlen(token->str)))
        ast->word = token->str;
    else
    {
        token_free_str_and_destroy(token);
        return false;
    }
    token_destroy(token);
    return true;
}

static void add_elements(struct for_command_ast *ast, char *word)
{
    struct elements *new = xcalloc(1, sizeof(struct elements));
    new->word = word;
    if (ast->elements)
    {
        struct elements *current = ast->elements;
        while (current->next)
            current = current->next;
        current->next = new;
    }
    else
    {
        ast->elements = new;
    }
}

static bool get_elements(struct queue *lexer, struct for_command_ast *ast)
{
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_OPERATOR && strcmp(token->str, ";") == 0)
    {
        queue_dequeue(lexer);
        token_free_str_and_destroy(token);
        return true;
    }
    skip_newlines(lexer);
    token = queue_dequeue(lexer);
    if (token->type == TOKEN_GENERIC && strcmp(token->str, "in") == 0)
    {
        token_free_str_and_destroy(token);
        token = queue_dequeue(lexer);
        while (token->type == TOKEN_GENERIC)
        {
            add_elements(ast, token->str);
            token_destroy(token);
            token = queue_dequeue(lexer);
        }
        if ((token->type == TOKEN_OPERATOR && strcmp(token->str, ";") == 0)
            || token->type == TOKEN_NEWLINE)
        {
            token_free_str_and_destroy(token);
            return true;
        }
    }
    token_free_str_and_destroy(token);
    return false;
}

static bool get_body(struct queue *lexer, struct hash_map *variables,
                     struct for_command_ast *ast)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    ast->body = compound_list_parse(lexer, variables);
    return ast->body;
}

void *for_command_parse(struct queue *lexer, struct hash_map *variables)
{
    struct for_command_ast *ast = for_command_ast_create();
    if (!get_first_word(lexer, ast))
        return destroy_and_nullify(ast, AST_TYPE_FOR);
    if (!get_elements(lexer, ast))
        return destroy_and_nullify(ast, AST_TYPE_FOR);
    skip_newlines(lexer);
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_GENERIC && strcmp(token->str, "do") == 0)
    {
        if (!get_body(lexer, variables, ast))
            return destroy_and_nullify(ast, AST_TYPE_FOR);
    }
    else
        return destroy_and_nullify(ast, AST_TYPE_FOR);
    if (next_is_keyword(lexer, "done"))
        return ast;
    return destroy_and_nullify(ast, AST_TYPE_FOR);
}
