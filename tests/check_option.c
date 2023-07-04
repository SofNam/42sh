#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "option.h"

START_TEST(test_no_option)
{
    char *argv[2];
    argv[0] = "42sh";
    argv[1] = NULL;
    struct option *option = get_option(1, argv, NULL);
    int no_option = 0;
    if (option->c->state == 0 && option->O->state == 0)
        no_option = 1;
    ck_assert_int_eq(no_option, 1);
    free_option(option);
}
END_TEST

START_TEST(test_c_simple_command)
{
    char *argv[4];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = "echo foo";
    argv[3] = NULL;
    struct option *option = get_option(3, argv, NULL);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    free_option(option);
}
END_TEST

START_TEST(test_c_alone)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->error, "-c: option requires an argument");
    free_option(option);
}
END_TEST

START_TEST(test_o_minus_simple_shopt)
{
    char *argv[4];
    argv[0] = "42sh";
    argv[1] = "-O";
    argv[2] = "xpg_echo";
    argv[3] = NULL;
    struct option *option = get_option(3, argv, NULL);
    ck_assert_int_eq(option->O->state, -1);
    ck_assert_str_eq(option->O->shopt, "xpg_echo");
    free_option(option);
}
END_TEST

START_TEST(test_o_plus_simple_shopt)
{
    char *argv[4];
    argv[0] = "42sh";
    argv[1] = "+O";
    argv[2] = "xpg_echo";
    argv[3] = NULL;
    struct option *option = get_option(3, argv, NULL);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert_str_eq(option->O->shopt, "xpg_echo");
    free_option(option);
}
END_TEST

START_TEST(test_c_o_minus_complex)
{
    char *argv[6];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = "echo foo";
    argv[3] = "-O";
    argv[4] = "xpg_echo";
    argv[5] = NULL;
    struct option *option = get_option(5, argv, NULL);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    free_option(option);
}
END_TEST

START_TEST(test_c_o_plus_complex)
{
    char *argv[6];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = "echo foo";
    argv[3] = "+O";
    argv[4] = "xpg_echo";
    argv[5] = NULL;
    struct option *option = get_option(5, argv, NULL);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    free_option(option);
}
END_TEST

START_TEST(test_error_case_o_minus_invalid_shell_option_name)
{
    char *argv[5];
    argv[0] = "42sh";
    argv[1] = "-O";
    argv[2] = "-c";
    argv[3] = "echo foo";
    argv[4] = NULL;
    struct option *option = get_option(4, argv, NULL);
    ck_assert_int_eq(option->O->state, -1);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert_str_eq(option->error, "-c: invalid shell option name");
    free_option(option);
}
END_TEST

START_TEST(test_error_case_o_plus_invalid_shell_option_name)
{
    char *argv[5];
    argv[0] = "42sh";
    argv[1] = "+O";
    argv[2] = "-c";
    argv[3] = "echo foo";
    argv[4] = NULL;
    struct option *option = get_option(4, argv, NULL);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert_str_eq(option->error, "-c: invalid shell option name");
    free_option(option);
}
END_TEST

START_TEST(test_error_case_o_minus_invalid_shell_option_name_2)
{
    char *argv[5];
    argv[0] = "42sh";
    argv[1] = "-O";
    argv[2] = "-potato";
    argv[3] = "echo foo";
    argv[4] = NULL;
    struct option *option = get_option(4, argv, NULL);
    ck_assert_int_eq(option->O->state, -1);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert_str_eq(option->error, "-potato: invalid shell option name");
    free_option(option);
}
END_TEST

START_TEST(test_error_case_o_plus_invalid_shell_option_name_2)
{
    char *argv[5];
    argv[0] = "42sh";
    argv[1] = "+O";
    argv[2] = "-potato";
    argv[3] = "echo foo";
    argv[4] = NULL;
    struct option *option = get_option(4, argv, NULL);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert_str_eq(option->error, "-potato: invalid shell option name");
    free_option(option);
}
END_TEST

START_TEST(test_error_case_o_plus_invalid_shell_option_name_3)
{
    char *argv[4];
    argv[0] = "42sh";
    argv[1] = "+O";
    argv[2] = "-potato";
    argv[3] = NULL;
    struct option *option = get_option(3, argv, NULL);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert_str_eq(option->error, "-potato: invalid shell option name");
    free_option(option);
}
END_TEST

START_TEST(test_case_o_minus_shopt_NULL)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "-O";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->O->state, -1);
    ck_assert(!option->O->shopt);
    ck_assert(!option->error);
    free_option(option);
}
END_TEST

START_TEST(test_case_o_plus_shopt_NULL)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "+O";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert(!option->O->shopt);
    ck_assert(!option->error);
    free_option(option);
}
END_TEST

START_TEST(test_c_double_command)
{
    char *argv[6];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = "echo foo";
    argv[3] = "-c";
    argv[4] = "echo hello world!";
    argv[5] = NULL;
    struct option *option = get_option(5, argv, NULL);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    free_option(option);
}
END_TEST

START_TEST(test_minus_o_double_command)
{
    char *argv[6];
    argv[0] = "42sh";
    argv[1] = "-O";
    argv[2] = "xpg_echo";
    argv[3] = "-O";
    argv[4] = "sourcepath";
    argv[5] = NULL;
    struct option *option = get_option(5, argv, NULL);
    ck_assert_int_eq(option->O->state, -1);
    ck_assert_str_eq(option->O->shopt, "xpg_echo");
    struct option_O *next = option->O->next;
    ck_assert_int_eq(next->state, -1);
    ck_assert_str_eq(next->shopt, "sourcepath");
    free_option(option);
}
END_TEST

START_TEST(test_only_filename)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "filename";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->O->state, 0);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert_str_eq(option->filename->filename, "filename");
    free_option(option);
}
END_TEST

START_TEST(test_minus_O_filename)
{
    char *argv[5];
    argv[0] = "42sh";
    argv[1] = "+O";
    argv[2] = "xpg_echo";
    argv[3] = "filename";
    argv[4] = NULL;
    struct option *option = get_option(4, argv, NULL);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert_str_eq(option->filename->filename, "filename");
    free_option(option);
}
END_TEST

START_TEST(test_c_double_command_filename)
{
    char *argv[7];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = "echo foo";
    argv[3] = "-c";
    argv[4] = "echo hello world!";
    argv[5] = "filename";
    argv[6] = NULL;
    struct option *option = get_option(6, argv, NULL);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    ck_assert(!option->filename->filename);
    free_option(option);
}
END_TEST

START_TEST(test_only_gnu_long_option)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "--verbose";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->O->state, 0);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert(!option->filename->filename);
    ck_assert(option->gnu->e_gnu == VERBOSE);
    free_option(option);
}
END_TEST

START_TEST(test_gnu_minus_O_filename)
{
    char *argv[6];
    argv[0] = "42sh";
    argv[1] = "--help";
    argv[2] = "-O";
    argv[3] = "xpg_echo";
    argv[4] = "file";
    argv[5] = NULL;
    struct option *option = get_option(5, argv, NULL);
    ck_assert(option->gnu->e_gnu == HELP);
    ck_assert_int_eq(option->O->state, -1);
    ck_assert_str_eq(option->O->shopt, "xpg_echo");
    ck_assert_str_eq(option->filename->filename, "file");
    ck_assert_int_eq(option->c->state, 0);
    ck_assert(option->gnu->position < option->O->position);
    ck_assert(option->gnu->position < option->filename->position);
    free_option(option);
}
END_TEST

START_TEST(test_gnu_plus_O_c)
{
    char *argv[7];
    argv[0] = "42sh";
    argv[1] = "--dump-po-strings";
    argv[2] = "+O";
    argv[3] = "xpg_echo";
    argv[4] = "-c";
    argv[5] = "echo foo";
    argv[6] = NULL;
    struct option *option = get_option(6, argv, NULL);
    ck_assert(option->gnu->e_gnu == DUMP_PO_STRINGS);
    ck_assert_int_eq(option->O->state, 1);
    ck_assert_str_eq(option->O->shopt, "xpg_echo");
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    ck_assert(!option->filename->filename);
    ck_assert(option->gnu->position < option->O->position);
    ck_assert(option->gnu->position < option->c->position);
    free_option(option);
}
END_TEST

START_TEST(test_gnu_c_plus_O_filename)
{
    char *argv[8];
    argv[0] = "42sh";
    argv[1] = "--dump-po-strings";
    argv[2] = "-c";
    argv[3] = "echo foo";
    argv[4] = "+O";
    argv[5] = "xpg_echo";
    argv[6] = "file";
    argv[7] = NULL;
    struct option *option = get_option(7, argv, NULL);
    ck_assert(option->gnu->e_gnu == DUMP_PO_STRINGS);
    ck_assert_int_eq(option->c->state, 1);
    ck_assert_str_eq(option->c->command, "echo foo");
    ck_assert_int_eq(option->O->state, 0);
    ck_assert(!option->O->shopt);
    ck_assert(!option->filename->filename);
    ck_assert(option->gnu->position < option->c->position);
    free_option(option);
}
END_TEST

START_TEST(test_gnu_long_option_invalid)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "--potato";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->O->state, 0);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert(!option->filename->filename);
    ck_assert_str_eq(option->error, "--potato: invalid option");
    free_option(option);
}
END_TEST

START_TEST(test_option_invalid)
{
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "-z";
    argv[2] = NULL;
    struct option *option = get_option(2, argv, NULL);
    ck_assert_int_eq(option->O->state, 0);
    ck_assert_int_eq(option->c->state, 0);
    ck_assert(!option->filename->filename);
    ck_assert_str_eq(option->error, "-z: invalid option");
    free_option(option);
}
END_TEST

Suite *options_suite(void)
{
    Suite *s;
    TCase *tc_core;
    // TCase *tc_limits;

    s = suite_create("Option");

    // Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_no_option);
    tcase_add_test(tc_core, test_c_simple_command);
    tcase_add_test(tc_core, test_c_alone);
    tcase_add_test(tc_core, test_o_minus_simple_shopt);
    tcase_add_test(tc_core, test_o_plus_simple_shopt);
    tcase_add_test(tc_core, test_c_o_minus_complex);
    tcase_add_test(tc_core, test_c_o_plus_complex);
    tcase_add_test(tc_core, test_error_case_o_minus_invalid_shell_option_name);
    tcase_add_test(tc_core, test_error_case_o_plus_invalid_shell_option_name);
    tcase_add_test(tc_core,
                   test_error_case_o_minus_invalid_shell_option_name_2);
    tcase_add_test(tc_core, test_error_case_o_plus_invalid_shell_option_name_2);
    tcase_add_test(tc_core, test_error_case_o_plus_invalid_shell_option_name_3);
    tcase_add_test(tc_core, test_case_o_minus_shopt_NULL);
    tcase_add_test(tc_core, test_case_o_plus_shopt_NULL);
    tcase_add_test(tc_core, test_c_double_command);
    tcase_add_test(tc_core, test_minus_o_double_command);
    tcase_add_test(tc_core, test_only_filename);
    tcase_add_test(tc_core, test_minus_O_filename);
    tcase_add_test(tc_core, test_c_double_command_filename);
    tcase_add_test(tc_core, test_only_gnu_long_option);
    tcase_add_test(tc_core, test_gnu_minus_O_filename);
    tcase_add_test(tc_core, test_gnu_plus_O_c);
    tcase_add_test(tc_core, test_gnu_c_plus_O_filename);
    tcase_add_test(tc_core, test_gnu_long_option_invalid);
    tcase_add_test(tc_core, test_option_invalid);
    suite_add_tcase(s, tc_core);

    return s;
}
