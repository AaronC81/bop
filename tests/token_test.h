#include <check.h>

#include "token.h"

START_TEST(token_to_digit) {
    uint8_t digit;

    ck_assert(bop_token_to_digit(BOP_TOKEN_0, &digit));
    ck_assert(digit == 0);

    ck_assert(bop_token_to_digit(BOP_TOKEN_5, &digit));
    ck_assert(digit == 5);

    ck_assert(bop_token_to_digit(BOP_TOKEN_9, &digit));
    ck_assert(digit == 9);

    ck_assert(!bop_token_to_digit(BOP_TOKEN_PLUS, &digit));
    ck_assert(!bop_token_to_digit(BOP_TOKEN_NONE, &digit));
}
END_TEST

TCase* token_tcase() {
    TCase* tcase = tcase_create("token");

    tcase_add_test(tcase, token_to_digit);

    return tcase;
}
