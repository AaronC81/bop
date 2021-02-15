#include "expression.h"
#define TAGGED_UNION_FUNCTION_DEFINITIONS
#include "tagged_union_generate.h"

#include "tagged_union_utils.h"

enum result expression_evaluate(struct expression *exp, bop_number *out) {
    TAGGED_UNION_SWITCH(expression, *exp) {
        TAGGED_UNION_CASE(expression, number, num_exp)
            *out = num_exp.n;
        TAGGED_UNION_ESAC

        TAGGED_UNION_CASE(expression, operator_add, add_exp)
            bop_number l, r;
            EXPRESSION_PROPAGATE(expression_evaluate(add_exp.left, &l))
            EXPRESSION_PROPAGATE(expression_evaluate(add_exp.right, &r))

            *out = l + r;
        TAGGED_UNION_ESAC
    }

    return RESULT_OK;
}
