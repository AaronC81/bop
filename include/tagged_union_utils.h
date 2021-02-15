#pragma once

#define _CONCAT(X, Y) X ## Y
#define CONCAT(X, Y) _CONCAT(X, Y)
#define CONCAT3(X, Y, Z) CONCAT(X, CONCAT(Y, Z))

#define BOP_TAGGED_UNION_SWITCH(name, exp) \
    struct name __bop_tagged_union_switch = (exp); \
    enum name##__tag __bop_tagged_union_switch_exp = (exp).tag; \
    switch (__bop_tagged_union_switch_exp)

#define BOP_TAGGED_UNION_CASE(name, variant, unpack) \
    case name##__tag_##variant: { \
        struct name##_##variant unpack = __bop_tagged_union_switch.value.variant;

#define BOP_TAGGED_UNION_ESAC \
    break; }

