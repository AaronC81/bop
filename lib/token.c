#include "token.h"

bool bop_token_to_digit(enum bop_token token, uint8_t *digit) {
    if (token >= BOP_TOKEN_0 && token <= BOP_TOKEN_9) {
        *digit = token - BOP_TOKEN_0;
        return true;
    }

    return false;
}
