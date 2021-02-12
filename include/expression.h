#pragma once

#include "token.h"
#include "number.h"
#include "result.h"

#define TAGGED_UNION_NAME expression
#define TAGGED_UNION_VARIANTS \
    VARIANT(number, { bop_number n; }) \
    VARIANT(operator_add, { struct expression *left; struct expression *right; })
#include "tagged_union_generate.h"

#define EXPRESSION_PROPAGATE(x) { \
    enum result __result = x; \
    if (__result != RESULT_OK) { \
        return __result; \
    } \
}

enum result expression_evaluate(struct expression *exp, bop_number *out);
