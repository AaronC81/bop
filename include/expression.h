#pragma once

#include "token.h"
#include "number.h"
#include "result.h"

#define BOP_TAGGED_UNION_NAME bop_expression
#define BOP_TAGGED_UNION_VARIANTS \
    VARIANT(number, { bop_number n; }) \
    VARIANT(operator_add, { struct bop_expression *left; struct bop_expression *right; }) \
    VARIANT(operator_divide, { struct bop_expression *top; struct bop_expression *bottom; }) \
    \
    VARIANT(unstructured, { struct bop_expression **children; uint16_t children_length; }) \
    VARIANT(token, { enum bop_token t; }) \
    VARIANT(placeholder, { })
#include "tagged_union_generate.h"

#define BOP_EXPRESSION_PROPAGATE(x) { \
    enum result __result = x; \
    if (__result != RESULT_OK) { \
        return __result; \
    } \
}

/**
 * Recursively evaluates the expression \p exp and places the output into
 * \p out if successful.
 * Only fully-upgraded expressions may be evaluated.
 */
enum result bop_expression_evaluate(struct bop_expression *exp, bop_number *out);

/**
 * Upgrades the expression \p in and places the upgraded copy into \p out if
 * successful. This process removes all unstructured nodes.
 */
enum result bop_expression_upgrade(struct bop_expression *in, struct bop_expression *out);

struct bop_expression_upgrade_parse_context {
    struct bop_expression *in;
    uint16_t index;
};

enum result bop_expression_upgrade_parse_level1(struct bop_expression_upgrade_parse_context *ctx, struct bop_expression *out);
enum result bop_expression_upgrade_parse_level2(struct bop_expression_upgrade_parse_context *ctx, struct bop_expression *out);
enum result bop_expression_upgrade_parse_level3(struct bop_expression_upgrade_parse_context *ctx, struct bop_expression *out);
