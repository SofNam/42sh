#ifndef CHECK_TESTS_H
#define CHECK_TESTS_H

#include <check.h>

Suite *lexer_suite(void);
Suite *parser_suite(void);
Suite *options_suite(void);
Suite *assignment_suite(void);
Suite *quoting_suite(void);

#endif /* ! CHECK_TEST_H */
