#include <check.h>
#include <string.h>

#include "lexer.h"
#include "queue.h"
#include "token.h"

START_TEST(test_quoting)
{
    char *input = "\"echo hello\"";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(head->quote == TOKEN_QUOTE_DOUBLE);
    ck_assert(strcmp(head->str, "echo hello") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_quoting_quote)
{
    char *input = "echo \"say hello\"";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *middle = lexer->head->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "echo") == 0);
    ck_assert(head->quote == TOKEN_QUOTE_NONE);
    ck_assert(middle->type == TOKEN_GENERIC);
    ck_assert(strcmp(middle->str, "say hello") == 0);
    ck_assert(middle->quote == TOKEN_QUOTE_DOUBLE);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_quoting_escape_quote)
{
    char *input = "echo \"say \\\"hello\"";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *middle = lexer->head->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "echo") == 0);
    ck_assert(head->quote == TOKEN_QUOTE_NONE);
    ck_assert(middle->type == TOKEN_GENERIC);
    ck_assert(strcmp(middle->str, "say \\\"hello") == 0);
    ck_assert(middle->quote == TOKEN_QUOTE_DOUBLE);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_single_quoting)
{
    char *input = "\'echo hello\'";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(head->quote == TOKEN_QUOTE_SINGLE);
    ck_assert(strcmp(head->str, "echo hello") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_single_quoting_quote)
{
    char *input = "echo \'say hello\'";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *middle = lexer->head->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "echo") == 0);
    ck_assert(head->quote == TOKEN_QUOTE_NONE);
    ck_assert(middle->type == TOKEN_GENERIC);
    ck_assert(strcmp(middle->str, "say hello") == 0);
    ck_assert(middle->quote == TOKEN_QUOTE_SINGLE);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_single_quoting_escape_quote)
{
    char *input = "echo \'say \\\'hello\'hi\'";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *center = lexer->head->next->element;
    struct token *center_right = lexer->head->next->next->element;
    struct token *far_right = lexer->head->next->next->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "echo") == 0);
    ck_assert(head->quote == TOKEN_QUOTE_NONE);
    ck_assert(center->type == TOKEN_GENERIC);
    ck_assert(strcmp(center->str, "say \\") == 0);
    ck_assert(center->quote == TOKEN_QUOTE_SINGLE);
    ck_assert(center_right->type == TOKEN_GENERIC);
    ck_assert(strcmp(center_right->str, "hello") == 0);
    ck_assert(center_right->quote == TOKEN_QUOTE_NONE);
    ck_assert(far_right->type == TOKEN_GENERIC);
    ck_assert(strcmp(far_right->str, "hi") == 0);
    ck_assert(far_right->quote == TOKEN_QUOTE_SINGLE);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

Suite *quoting_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Quoting");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_quoting);
    suite_add_tcase(s, tc_core);

    return s;
}
