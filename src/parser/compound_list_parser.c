#include <string.h>

#include "destroy_and_nullify.h"
#include "lexer.h"
#include "parser.h"
#include "skip_newlines.h"
#include "token.h"

static bool is_shell_command_keyword(const char *keyword)
{
    return strcmp(keyword, "do") == 0 || strcmp(keyword, "done") == 0
        || strcmp(keyword, "then") == 0 || strcmp(keyword, "else") == 0
        || strcmp(keyword, "elif") == 0 || strcmp(keyword, "fi") == 0
        || strcmp(keyword, "case") == 0 || strcmp(keyword, "esac") == 0;
}

static bool is_closing_command_keyword(const char *keyword)
{
    return strcmp(keyword, "done") == 0 || strcmp(keyword, "then") == 0
        || strcmp(keyword, "else") == 0 || strcmp(keyword, "elif") == 0
        || strcmp(keyword, "fi") == 0 || strcmp(keyword, "esac") == 0;
}

static struct compound_list_ast *
compound_list_parse_rec(struct queue *lexer, struct hash_map *variables)
{
    struct compound_list_ast *ast = compound_list_ast_create();
    skip_newlines(lexer);
    struct token *token = queue_peek(lexer);
    if (token->type == TOKEN_GENERIC && is_closing_command_keyword(token->str))
        return destroy_and_nullify(ast, AST_TYPE_COMPOUND);
    if ((ast->ast = and_or_parse(lexer, variables)) == NULL)
        return destroy_and_nullify(ast, AST_TYPE_COMPOUND);
    token = queue_peek(lexer);
    if ((token->type == TOKEN_OPERATOR
         && (strcmp(token->str, ";") == 0 || strcmp(token->str, "&") == 0))
        || token->type == TOKEN_NEWLINE)
    {
        get_next_token(lexer);
        skip_newlines(lexer);
        token = queue_peek(lexer);
        if (token->type == TOKEN_GENERIC
            && !is_shell_command_keyword(token->str))
            ast->next = compound_list_parse_rec(lexer, variables);
    }
    skip_newlines(lexer);
    return ast;
}

void *compound_list_parse(struct queue *lexer, struct hash_map *variables)
{
    struct compound_list_ast *ast = compound_list_parse_rec(lexer, variables);
    return ast;
}
