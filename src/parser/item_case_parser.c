#include <string.h>

#include "destroy_and_nullify.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

struct item_case *item_compound_list_parse(struct queue *lexer,
                                           struct hash_map *variables,
                                           struct item_case *item_case)
{
    struct token *token = queue_dequeue(lexer);
    token_free_str_and_destroy(token);
    skip_newlines(lexer);
    token = queue_peek(lexer);
    if ((token->type == TOKEN_OPERATOR && strcmp(token->str, ";;") == 0)
        || (token->type == TOKEN_GENERIC && strcmp(token->str, "esac") == 0))
        return item_case;
    item_case->compound_list = compound_list_parse(lexer, variables);
    if (item_case->compound_list == NULL)
        return destroy_and_nullify(item_case, AST_TYPE_CASE_ITEM);
    return item_case;
}

void *item_parse(struct queue *lexer, struct hash_map *variables)
{
    struct item_case *item_case = item_case_create();
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_OPERATOR && strcmp(token->str, "(") == 0)
        token = get_next_token(lexer);
    if (token->type != TOKEN_GENERIC)
        return destroy_and_nullify(item_case, AST_TYPE_CASE_ITEM);
    string_vector_add(item_case->patterns, token->str);
    token_destroy(token);
    queue_dequeue(lexer);
    token = queue_peek(lexer);
    while (token->type == TOKEN_OPERATOR && strcmp(token->str, "|") == 0)
    {
        token = get_next_token(lexer);
        if (token->type != TOKEN_GENERIC)
            return destroy_and_nullify(item_case, AST_TYPE_CASE_ITEM);
        string_vector_add(item_case->patterns, token->str);
        token_destroy(token);
        queue_dequeue(lexer);
        token = queue_peek(lexer);
    }
    if (token->type != TOKEN_OPERATOR || strcmp(token->str, ")") != 0)
        return destroy_and_nullify(item_case, AST_TYPE_CASE_ITEM);
    return item_compound_list_parse(lexer, variables, item_case);
}
