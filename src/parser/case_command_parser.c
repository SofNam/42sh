#include <string.h>

#include "destroy_and_nullify.h"
#include "next_is_keyword.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

static char *get_word(struct queue *lexer)
{
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_GENERIC)
    {
        char *word = token->str;
        queue_dequeue(lexer);
        token_destroy(token);
        return word;
    }
    else
    {
        return NULL;
    }
}

static bool clause_parse(struct queue *lexer, struct hash_map *variables,
                         struct case_command_ast *ast)
{
    skip_newlines(lexer);
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_GENERIC && strcmp(token->str, "esac") == 0)
        return true;
    struct item_case *item_case = item_parse(lexer, variables);
    if (item_case == NULL)
        return false;
    vector_add(ast->items, item_case);
    token = queue_peek(lexer);
    while (token->type == TOKEN_OPERATOR && strcmp(token->str, ";;") == 0)
    {
        queue_dequeue(lexer);
        token_free_str_and_destroy(token);
        skip_newlines(lexer);
        token = queue_peek(lexer);
        if ((token->type != TOKEN_OPERATOR || strcmp(token->str, "(") != 0)
            && (token->type != TOKEN_GENERIC
                || strcmp(token->str, "esac") == 0))
            break;
        item_case = item_parse(lexer, variables);
        if (item_case == NULL)
            return false;
        vector_add(ast->items, item_case);
        token = queue_peek(lexer);
    }
    return true;
}

void *case_command_parse(struct queue *lexer, struct hash_map *variables)
{
    struct case_command_ast *ast = case_command_ast_create();
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    ast->word = get_word(lexer);
    if (ast->word == NULL)
        return destroy_and_nullify(ast, AST_TYPE_CASE);
    skip_newlines(lexer);
    if (!next_is_keyword(lexer, "in"))
        return destroy_and_nullify(ast, AST_TYPE_CASE);
    skip_newlines(lexer);
    if (!clause_parse(lexer, variables, ast))
        return destroy_and_nullify(ast, AST_TYPE_CASE);
    if (!next_is_keyword(lexer, "esac"))
        return destroy_and_nullify(ast, AST_TYPE_CASE);
    return ast;
}
