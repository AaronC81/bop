#include "expression.h"
#define BOP_TAGGED_UNION_FUNCTION_DEFINITIONS
#include "tagged_union_generate.h"

#include "tagged_union_utils.h"

enum result bop_expression_evaluate(struct bop_expression *exp, bop_number *out) {
    BOP_TAGGED_UNION_SWITCH(bop_expression, *exp) {
        BOP_TAGGED_UNION_CASE(bop_expression, number, num_exp)
            *out = num_exp.n;
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, operator_add, add_exp)
            bop_number l, r;
            BOP_EXPRESSION_PROPAGATE(bop_expression_evaluate(add_exp.left, &l))
            BOP_EXPRESSION_PROPAGATE(bop_expression_evaluate(add_exp.right, &r))

            *out = l + r;
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, operator_divide, div_exp)
            bop_number t, b;
            BOP_EXPRESSION_PROPAGATE(bop_expression_evaluate(div_exp.top, &t))
            BOP_EXPRESSION_PROPAGATE(bop_expression_evaluate(div_exp.bottom, &b))

            *out = t / b;
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, unstructured, _)
            return RESULT_EVALUATE_UNSTRUCTURED;
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, token, _)
            return RESULT_EVALUATE_TOKEN;
        BOP_TAGGED_UNION_ESAC
        
        BOP_TAGGED_UNION_CASE(bop_expression, placeholder, _)
            return RESULT_EVALUATE_PLACEHOLDER;
        BOP_TAGGED_UNION_ESAC
    }

    return RESULT_OK;
}
