#include <check.h>

#include "token_test.h"
#include "expression_test.h"

int main() {
    // Build test suite and runner
    Suite *suite = suite_create("bop");

    suite_add_tcase(suite, token_tcase());
    suite_add_tcase(suite, expression_tcase());

    SRunner *suite_runner = srunner_create(suite);

    // Run tests
    srunner_run_all(suite_runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);
    return number_failed;
}
