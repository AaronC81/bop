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
    VARIANT(unstructured, { struct bop_expression **children; }) \
    VARIANT(token, { enum bop_token t; }) \
    VARIANT(placeholder, { })
#include "tagged_union_generate.h"

#define BOP_EXPRESSION_PROPAGATE(x) { \
    enum result __result = x; \
    if (__result != RESULT_OK) { \
        return __result; \
    } \
}

enum result bop_expression_evaluate(struct bop_expression *exp, bop_number *out);
