#include <check.h>

#include "expression.h"

START_TEST(expression_evaluate_number) {
    struct expression number = expression_new_number((struct expression_number){ .n = 3.4 });
    bop_number n;
    fail_unless(expression_evaluate(&number, &n) == RESULT_OK);
    fail_unless(n == 3.4);
}
END_TEST

TCase* expression_tcase() {
    TCase* tcase = tcase_create("expression");

    tcase_add_test(tcase, expression_evaluate_number);

    return tcase;
}
