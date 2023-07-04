#include <stdlib.h>

#include "builtins.h"
#include "check_tests.h"

struct global global;

int main(void)
{
    SRunner *sr = srunner_create(lexer_suite());
    srunner_add_suite(sr, parser_suite());
    srunner_add_suite(sr, options_suite());
    srunner_add_suite(sr, assignment_suite());
    srunner_add_suite(sr, quoting_suite());
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
