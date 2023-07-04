#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

START_TEST(test_assignment_one)
{
    struct hash_map *variables = hash_map_init();
    struct queue *lexer = lex("var=foo");
    struct command_ast *ast = command_parse(lexer, variables);
    struct simple_command_ast *sca = ast->ast;
    ck_assert(sca->argv->size == 0);
    char *value = hash_map_get(variables, "var");
    ck_assert(strcmp(value, "foo") == 0);
    command_exec(ast);
    hash_map_destroy(variables);
}
END_TEST

START_TEST(test_assignment_multiple)
{
    struct hash_map *variables = hash_map_init();
    struct queue *lexer = lex("var=foo bar=lol yo=haha=bonjour");
    struct command_ast *ast = command_parse(lexer, variables);
    struct simple_command_ast *sca = ast->ast;
    ck_assert(sca->argv->size == 0);
    ck_assert(strcmp(hash_map_get(variables, "var"), "foo") == 0);
    ck_assert(strcmp(hash_map_get(variables, "bar"), "lol") == 0);
    ck_assert(strcmp(hash_map_get(variables, "yo"), "haha=bonjour") == 0);
    command_exec(ast);
    hash_map_destroy(variables);
}
END_TEST

START_TEST(test_assignment_and_simple_command)
{
    struct hash_map *variables = hash_map_init();
    struct queue *lexer = lex("var=foo ls");
    struct command_ast *ast = command_parse(lexer, variables);
    struct simple_command_ast *sca = ast->ast;
    ck_assert(sca->argv->size == 1);
    ck_assert(strcmp(sca->argv->strings[0], "ls") == 0);
    ck_assert(strcmp(hash_map_get(variables, "var"), "foo") == 0);
    command_exec(ast);
    hash_map_destroy(variables);
}
END_TEST

START_TEST(test_assignment_invalid_name)
{
    struct hash_map *variables = hash_map_init();
    struct queue *lexer = lex("9var=foo");
    struct command_ast *ast = command_parse(lexer, variables);
    struct simple_command_ast *sca = ast->ast;
    ck_assert(sca->argv->size == 1);
    ck_assert(strcmp(sca->argv->strings[0], "9var=foo") == 0);
    ck_assert(hash_map_get(variables, "var") == NULL);
    command_exec(ast);
    hash_map_destroy(variables);
}
END_TEST

START_TEST(test_assignment_valid_and_invalid)
{
    struct hash_map *variables = hash_map_init();
    struct queue *lexer = lex("var=foo 9var=bar ls lol=lmao");
    struct command_ast *ast = command_parse(lexer, variables);
    struct simple_command_ast *sca = ast->ast;
    ck_assert(sca->argv->size == 3);
    ck_assert(strcmp(sca->argv->strings[0], "9var=bar") == 0);
    ck_assert(strcmp(sca->argv->strings[1], "ls") == 0);
    ck_assert(strcmp(sca->argv->strings[2], "lol=lmao") == 0);
    ck_assert(strcmp(hash_map_get(variables, "var"), "foo") == 0);
    ck_assert(hash_map_get(variables, "9var") == NULL);
    ck_assert(hash_map_get(variables, "lol") == NULL);
    command_exec(ast);
    hash_map_destroy(variables);
}
END_TEST

Suite *assignment_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Assignment");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_assignment_one);
    tcase_add_test(tc_core, test_assignment_multiple);
    tcase_add_test(tc_core, test_assignment_and_simple_command);
    tcase_add_test(tc_core, test_assignment_invalid_name);
    tcase_add_test(tc_core, test_assignment_valid_and_invalid);
    suite_add_tcase(s, tc_core);

    return s;
}
