#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Represents a token in an input.
 */
enum bop_token {
    BOP_TOKEN_NONE = 0,
    BOP_TOKEN_0,
    BOP_TOKEN_1,
    BOP_TOKEN_2,
    BOP_TOKEN_3,
    BOP_TOKEN_4,
    BOP_TOKEN_5,
    BOP_TOKEN_6,
    BOP_TOKEN_7,
    BOP_TOKEN_8,
    BOP_TOKEN_9,
    BOP_TOKEN_LPAREN,
    BOP_TOKEN_RPAREN,
    BOP_TOKEN_PLUS,
    BOP_TOKEN_MINUS,
    BOP_TOKEN_MULTIPLY,
    BOP_TOKEN_DIVIDE,
};

/**
 * If \p token is a token representing a single numerical digit, sets \p digit
 * to that digit and returns true.
 */
bool bop_token_to_digit(enum bop_token token, uint8_t *digit);
