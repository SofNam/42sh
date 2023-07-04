#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "token.h"

START_TEST(test_parser_one_word)
{
    char *input = "bugleweed";
    struct queue *lexer = lex(input);
    struct list_ast *ast = list_parse(lexer, NULL);
    ck_assert(ast->next == NULL);
    ck_assert(ast->ast->type == AND_OR_NONE);
    struct command_ast *command = ast->ast->ast->commands->data[0];
    ck_assert(command->type == SIMPLE_COMMAND_AST);
    struct simple_command_ast *sc_ast = command->ast;
    ck_assert(strcmp(sc_ast->argv->strings[0], "bugleweed") == 0);
    list_ast_destroy(ast);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_parser_semicolon)
{
    char *input = "echo lol ;pwd";
    struct queue *lexer = lex(input);
    struct list_ast *ast = list_parse(lexer, NULL);
    ck_assert(ast->next != NULL);
    list_ast_destroy(ast);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_parser_semicolon_only)
{
    char *input = ";";
    struct queue *lexer = lex(input);
    struct list_ast *ast = list_parse(lexer, NULL);
    ck_assert(ast == NULL);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_parser_and_simple)
{
    char *input = "ls && pwd";
    struct queue *lexer = lex(input);
    struct list_ast *ast = list_parse(lexer, NULL);
    ck_assert(ast != NULL);
    ck_assert(ast->ast->type == AND);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_parser_and_error)
{
    char *input = "&&";
    struct queue *lexer = lex(input);
    struct list_ast *ast = list_parse(lexer, NULL);
    ck_assert(ast == NULL);
    queue_destroy(lexer);
}
END_TEST

Suite *parser_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Parser");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_parser_one_word);
    tcase_add_test(tc_core, test_parser_semicolon);
    tcase_add_test(tc_core, test_parser_semicolon_only);
    tcase_add_test(tc_core, test_parser_and_simple);
    tcase_add_test(tc_core, test_parser_and_error);

    suite_add_tcase(s, tc_core);

    return s;
}
