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
    struct bop_expression t = bop_expression_new_operator_divide((struct bop_expression_operator_divide){ .left = &t1, .right = &t2 });
    struct bop_expression div = bop_expression_new_operator_divide((struct bop_expression_operator_divide){ .left = &t, .right = &b });
    
    bop_number n;
    fail_unless(bop_expression_evaluate(&t, &n) == RESULT_OK);
    fail_unless(n == 4.0);
    fail_unless(bop_expression_evaluate(&div, &n) == RESULT_OK);
    fail_unless(n == 1.6);
}
END_TEST

START_TEST(bop_expression_upgrade_no_change) {
    struct bop_expression input_number = bop_expression_new_number((struct bop_expression_number){ .n = 2.5 });
    struct bop_expression in = bop_expression_new_unstructured((struct bop_expression_unstructured){
        .children = (struct bop_expression*[]){ &input_number },
        .children_length = 1,
    });
    struct bop_expression out;

    fail_unless(bop_expression_upgrade(&in, &out) == RESULT_OK);
    struct bop_expression_number n;
    fail_unless(bop_expression_is_number(out, &n));
    fail_unless(n.n == 2.5);
}
END_TEST

START_TEST(bop_expression_upgrade_simple_addition) {
    struct bop_expression left = bop_expression_new_number((struct bop_expression_number){ .n = 2.5 });
    struct bop_expression plus = bop_expression_new_token((struct bop_expression_token){ .t = BOP_TOKEN_PLUS });
    struct bop_expression right = bop_expression_new_number((struct bop_expression_number){ .n = 4.2 });

    struct bop_expression in = bop_expression_new_unstructured((struct bop_expression_unstructured){
        .children = (struct bop_expression*[]){ &left, &plus, &right },
        .children_length = 3,
    });
    struct bop_expression out;

    fail_unless(bop_expression_upgrade(&in, &out) == RESULT_OK);

    struct bop_expression_operator_add a;
    struct bop_expression_number n;

    fail_unless(bop_expression_is_operator_add(out, &a));
    fail_unless(bop_expression_is_number(*a.left, &n));
    fail_unless(n.n == 2.5);
    fail_unless(bop_expression_is_number(*a.right, &n));
    fail_unless(n.n == 4.2);
}
END_TEST

START_TEST(bop_expression_upgrade_associativity) {
    struct bop_expression a = bop_expression_new_number((struct bop_expression_number){ .n = 8 });
    struct bop_expression b = bop_expression_new_token((struct bop_expression_token){ .t = BOP_TOKEN_DIVIDE });
    struct bop_expression c = bop_expression_new_number((struct bop_expression_number){ .n = 4 });
    struct bop_expression d = bop_expression_new_token((struct bop_expression_token){ .t = BOP_TOKEN_DIVIDE });
    struct bop_expression e = bop_expression_new_number((struct bop_expression_number){ .n = 2 });

    struct bop_expression in = bop_expression_new_unstructured((struct bop_expression_unstructured){
        .children = (struct bop_expression*[]){ &a, &b, &c, &d, &e },
        .children_length = 5,
    });
    struct bop_expression out;

    fail_unless(bop_expression_upgrade(&in, &out) == RESULT_OK);

    struct bop_expression_operator_divide div;
    struct bop_expression_number n;

    fail_unless(bop_expression_is_operator_divide(out, &div));
    fail_unless(bop_expression_is_number(*div.right, &n));
    fail_unless(n.n == 2);

    fail_unless(bop_expression_is_operator_divide(*div.left, &div));
    fail_unless(bop_expression_is_number(*div.left, &n));
    fail_unless(n.n == 8);
    fail_unless(bop_expression_is_number(*div.right, &n));
    fail_unless(n.n == 4);
}
END_TEST

START_TEST(bop_expression_upgrade_digit_tokens) {
    struct bop_expression *in = bop_expression_new_unstructured_from_tokens((enum bop_token[]){
        BOP_TOKEN_1,
        BOP_TOKEN_2,
        BOP_TOKEN_3,
        BOP_TOKEN_PLUS,
        BOP_TOKEN_4,
    }, 5);
    struct bop_expression out;

    fail_unless(bop_expression_upgrade(in, &out) == RESULT_OK);

    struct bop_expression_operator_add add;
    struct bop_expression_number n;
    fail_unless(bop_expression_is_operator_add(out, &add));
    fail_unless(bop_expression_is_number(*add.left, &n));
    fail_unless(n.n == 123);
    fail_unless(bop_expression_is_number(*add.right, &n));
    fail_unless(n.n == 4);
}
END_TEST


TCase* bop_expression_tcase() {
    TCase* tcase = tcase_create("bop_expression");

    tcase_add_test(tcase, bop_expression_evaluate_number);
    tcase_add_test(tcase, bop_expression_evaluate_operator_add);
    tcase_add_test(tcase, bop_expression_evaluate_operator_divide);

    tcase_add_test(tcase, bop_expression_upgrade_no_change);
    tcase_add_test(tcase, bop_expression_upgrade_simple_addition);
    tcase_add_test(tcase, bop_expression_upgrade_associativity);
    tcase_add_test(tcase, bop_expression_upgrade_digit_tokens);

    return tcase;
}
