#include "expression.h"
#define BOP_TAGGED_UNION_FUNCTION_DEFINITIONS
#include "tagged_union_generate.h"

#include "tagged_union_utils.h"

#include <stdlib.h>

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
            bop_number l, r;
            BOP_EXPRESSION_PROPAGATE(bop_expression_evaluate(div_exp.left, &l))
            BOP_EXPRESSION_PROPAGATE(bop_expression_evaluate(div_exp.right, &r))

            *out = l / r;
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


enum result bop_expression_upgrade(struct bop_expression *in, struct bop_expression *out) {
    BOP_TAGGED_UNION_SWITCH(bop_expression, *in) {
        BOP_TAGGED_UNION_CASE(bop_expression, number, _)
            *out = *in;
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, operator_add, add_exp)
            *out = *in;
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade(add_exp.left, out->value.operator_add.left))
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade(add_exp.right, out->value.operator_add.right))
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, operator_divide, div_exp)
            *out = *in;
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade(div_exp.left, out->value.operator_divide.left))
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade(div_exp.right, out->value.operator_divide.right))
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, unstructured, _)
            // Construct a parser context
            struct bop_expression_upgrade_parse_context ctx = {
                .in = in,
                .index = 0,
            };

            // Let's go!
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level1(&ctx, out));
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, token, _)
            return RESULT_UPGRADE_TOKEN;
        BOP_TAGGED_UNION_ESAC

        BOP_TAGGED_UNION_CASE(bop_expression, placeholder, _)
            return RESULT_UPGRADE_PLACEHOLDER;
        BOP_TAGGED_UNION_ESAC
    }

    return RESULT_OK;
}

#define PARSE_CURR (ctx->in->value.unstructured.children[ctx->index])
#define PARSE_NEXT (ctx->index++)
#define PARSE_EOI (ctx->index >= ctx->in->value.unstructured.children_length)

enum result bop_expression_upgrade_parse_level1(struct bop_expression_upgrade_parse_context *ctx, struct bop_expression *out) {
    BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level2(ctx, out));

    struct bop_expression_token token;
    while (!PARSE_EOI
        && bop_expression_is_token(*PARSE_CURR, &token)
        && (token.t == BOP_TOKEN_PLUS || token.t == BOP_TOKEN_MINUS)) {

        enum bop_token this_token = token.t;

        // Discard this token
        PARSE_NEXT;

        // Copy the old output node, which will become the left side, into a new
        // malloced location
        // Also malloc memory for the right
        struct bop_expression *left = malloc(sizeof(struct bop_expression));
        struct bop_expression *right = malloc(sizeof(struct bop_expression));
        *left = *out;

        // Construct an addition/subtraction and parse the right side
        if (this_token == BOP_TOKEN_PLUS) {
            *out = bop_expression_new_operator_add((struct bop_expression_operator_add){
                .left = left,
                .right = right,
            });
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level2(ctx, out->value.operator_add.right));
        } else {
            *out = bop_expression_new_operator_subtract((struct bop_expression_operator_subtract){
                .left = left,
                .right = right,
            });
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level2(ctx, out->value.operator_subtract.right));
        }
    }

    return RESULT_OK;
}

enum result bop_expression_upgrade_parse_level2(struct bop_expression_upgrade_parse_context *ctx, struct bop_expression *out) {
    BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level3(ctx, out));

    struct bop_expression_token token;
    while (!PARSE_EOI
        && bop_expression_is_token(*PARSE_CURR, &token)
        && (token.t == BOP_TOKEN_MULTIPLY || token.t == BOP_TOKEN_DIVIDE)) {

        enum bop_token this_token = token.t;

        // Discard this token
        PARSE_NEXT;

        // Copy the old output node, which will become the left side, into a new
        // malloced location
        struct bop_expression *left = malloc(sizeof(struct bop_expression));
        struct bop_expression *right = malloc(sizeof(struct bop_expression));
        *left = *out;

        // Construct a multiplication/division and parse the right side
        if (this_token == BOP_TOKEN_MULTIPLY) {
            *out = bop_expression_new_operator_multiply((struct bop_expression_operator_multiply){
                .left = left,
                .right = right,
            });
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level3(ctx, out->value.operator_multiply.right));
        } else {
            *out = bop_expression_new_operator_divide((struct bop_expression_operator_divide){
                .left = left,
                .right = right,
            });
            BOP_EXPRESSION_PROPAGATE(bop_expression_upgrade_parse_level3(ctx, out->value.operator_divide.right));
        }
    }

    return RESULT_OK;
}

enum result bop_expression_upgrade_parse_level3(struct bop_expression_upgrade_parse_context *ctx, struct bop_expression *out) {
    // TODO: construct number from tokens if needed
    
    if (!PARSE_EOI && bop_expression_is_number(*PARSE_CURR, NULL)) {
        *out = *PARSE_CURR;
        PARSE_NEXT;
        return RESULT_OK;
    }

    return RESULT_SYNTAX_ERROR;
}
