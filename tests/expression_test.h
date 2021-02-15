#include <check.h>

#include "expression.h"

START_TEST(bop_expression_evaluate_number) {
    struct bop_expression number = bop_expression_new_number((struct bop_expression_number){ .n = 3.4 });
    bop_number n;
    fail_unless(bop_expression_evaluate(&number, &n) == RESULT_OK);
    fail_unless(n == 3.4);
}
END_TEST

START_TEST(bop_expression_evaluate_operator_add) {
    struct bop_expression l = bop_expression_new_number((struct bop_expression_number){ .n = 1.5 });
    struct bop_expression r1 = bop_expression_new_number((struct bop_expression_number){ .n = 2.1 });
    struct bop_expression r2 = bop_expression_new_number((struct bop_expression_number){ .n = 4.9 });
    struct bop_expression r = bop_expression_new_operator_add((struct bop_expression_operator_add){ .left = &r1, .right = &r2 });
    struct bop_expression add = bop_expression_new_operator_add((struct bop_expression_operator_add){ .left = &l, .right = &r });
    
    bop_number n;
    fail_unless(bop_expression_evaluate(&r, &n) == RESULT_OK);
    fail_unless(n == 7.0);
    fail_unless(bop_expression_evaluate(&add, &n) == RESULT_OK);
    fail_unless(n == 8.5);
}
END_TEST

START_TEST(bop_expression_evaluate_operator_divide) {
    struct bop_expression b = bop_expression_new_number((struct bop_expression_number){ .n = 2.5 });
    struct bop_expression t1 = bop_expression_new_number((struct bop_expression_number){ .n = 13 });
    struct bop_expression t2 = bop_expression_new_number((struct bop_expression_number){ .n = 3.25 });
    struct bop_expression t = bop_expression_new_operator_divide((struct bop_expression_operator_divide){ .top = &t1, .bottom = &t2 });
    struct bop_expression div = bop_expression_new_operator_divide((struct bop_expression_operator_divide){ .top = &t, .bottom = &b });
    
    bop_number n;
    fail_unless(bop_expression_evaluate(&t, &n) == RESULT_OK);
    fail_unless(n == 4.0);
    fail_unless(bop_expression_evaluate(&div, &n) == RESULT_OK);
    fail_unless(n == 1.6);
}
END_TEST


TCase* bop_expression_tcase() {
    TCase* tcase = tcase_create("bop_expression");

    tcase_add_test(tcase, bop_expression_evaluate_number);
    tcase_add_test(tcase, bop_expression_evaluate_operator_add);
    tcase_add_test(tcase, bop_expression_evaluate_operator_divide);

    return tcase;
}
