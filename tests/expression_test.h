#include <check.h>

#include "expression.h"

START_TEST(expression_evaluate_number) {
    struct expression number = expression_new_number((struct expression_number){ .n = 3.4 });
    bop_number n;
    fail_unless(expression_evaluate(&number, &n) == RESULT_OK);
    fail_unless(n == 3.4);
}
END_TEST

START_TEST(expression_evaluate_operator_add) {
    struct expression l = expression_new_number((struct expression_number){ .n = 1.5 });
    struct expression r1 = expression_new_number((struct expression_number){ .n = 2.1 });
    struct expression r2 = expression_new_number((struct expression_number){ .n = 4.9 });
    struct expression r = expression_new_operator_add((struct expression_operator_add){ .left = &r1, .right = &r2 });
    struct expression add = expression_new_operator_add((struct expression_operator_add){ .left = &l, .right = &r });
    
    bop_number n;
    fail_unless(expression_evaluate(&r, &n) == RESULT_OK);
    fail_unless(n == 7.0);
    fail_unless(expression_evaluate(&add, &n) == RESULT_OK);
    fail_unless(n == 8.5);
}
END_TEST


TCase* expression_tcase() {
    TCase* tcase = tcase_create("expression");

    tcase_add_test(tcase, expression_evaluate_number);
    tcase_add_test(tcase, expression_evaluate_operator_add);

    return tcase;
}
