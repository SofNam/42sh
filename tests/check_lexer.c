#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "queue.h"
#include "token.h"

START_TEST(test_lexer_init_empty_input)
{
    char *input = "";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head == tail);
    ck_assert(head->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_one_word)
{
    char *input = "I";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "I") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_two_words)
{
    char *input = "I am";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *body = lexer->head->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "I") == 0);
    ck_assert(body->type == TOKEN_GENERIC);
    ck_assert(strcmp(body->str, "am") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_first_last_token)
{
    char *input = "Curabitur vulputate vestibulum lorem.";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *penultimate = lexer->head->next->next->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "Curabitur") == 0);
    ck_assert(penultimate->type == TOKEN_GENERIC);
    ck_assert(strcmp(penultimate->str, "lorem.") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_semicolon)
{
    char *input = "ls; pwd";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *middle = lexer->head->next->element;
    struct token *penultimate = lexer->head->next->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "ls") == 0);
    ck_assert(middle->type == TOKEN_OPERATOR);
    ck_assert(strcmp(middle->str, ";") == 0);
    ck_assert(penultimate->type == TOKEN_GENERIC);
    ck_assert(strcmp(penultimate->str, "pwd") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_or)
{
    char *input = "ls||pwd";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *middle = lexer->head->next->element;
    struct token *penultimate = lexer->head->next->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "ls") == 0);
    ck_assert(middle->type == TOKEN_OPERATOR);
    ck_assert(strcmp(middle->str, "||") == 0);
    ck_assert(penultimate->type == TOKEN_GENERIC);
    ck_assert(strcmp(penultimate->str, "pwd") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_multiple_boolean)
{
    char *input = "ls && pwd || echo";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *center = lexer->head->next->element;
    struct token *center_right = lexer->head->next->next->element;
    struct token *right = lexer->head->next->next->next->element;
    struct token *far_right = lexer->head->next->next->next->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "ls") == 0);
    ck_assert(center->type == TOKEN_OPERATOR);
    ck_assert(strcmp(center->str, "&&") == 0);
    ck_assert(center_right->type == TOKEN_GENERIC);
    ck_assert(strcmp(center_right->str, "pwd") == 0);
    ck_assert(right->type == TOKEN_OPERATOR);
    ck_assert(strcmp(right->str, "||") == 0);
    ck_assert(far_right->type == TOKEN_GENERIC);
    ck_assert(strcmp(far_right->str, "echo") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

START_TEST(test_lexer_redirection)
{
    char *input = "ls 2> yo";
    struct queue *lexer = lex(input);
    struct token *head = lexer->head->element;
    struct token *middle = lexer->head->next->element;
    struct token *middle_next = lexer->head->next->next->element;
    struct token *penultimate = lexer->head->next->next->next->element;
    struct token *tail = lexer->tail->element;
    ck_assert(head->type == TOKEN_GENERIC);
    ck_assert(strcmp(head->str, "ls") == 0);
    ck_assert(middle->type == TOKEN_IONUMBER);
    ck_assert(strcmp(middle->str, "2") == 0);
    ck_assert(middle_next->type == TOKEN_REDIROP);
    ck_assert(strcmp(middle_next->str, ">") == 0);
    ck_assert(penultimate->type == TOKEN_GENERIC);
    ck_assert(strcmp(penultimate->str, "yo") == 0);
    ck_assert(tail->type == TOKEN_EOF);
    queue_destroy(lexer);
}
END_TEST

Suite *lexer_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Lexer");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_lexer_init_empty_input);
    tcase_add_test(tc_core, test_lexer_one_word);
    tcase_add_test(tc_core, test_lexer_two_words);
    tcase_add_test(tc_core, test_lexer_first_last_token);
    tcase_add_test(tc_core, test_lexer_semicolon);
    tcase_add_test(tc_core, test_lexer_or);
    tcase_add_test(tc_core, test_lexer_multiple_boolean);
    tcase_add_test(tc_core, test_lexer_redirection);
    suite_add_tcase(s, tc_core);

    return s;
}
