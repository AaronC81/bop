#include <stdbool.h>
#include <stddef.h>

#define _CONCAT(X, Y) X ## Y
#define CONCAT(X, Y) _CONCAT(X, Y)
#define CONCAT3(X, Y, Z) CONCAT(X, CONCAT(Y, Z))

#ifndef BOP_TAGGED_UNION_FUNCTION_DEFINITIONS

// Forward-define the main struct
struct BOP_TAGGED_UNION_NAME;

// Define the structs
#define VARIANT(name, contents) struct CONCAT3(BOP_TAGGED_UNION_NAME, _, name) contents;
BOP_TAGGED_UNION_VARIANTS
#undef VARIANT

// Define the union
#define VARIANT(name, contents) struct CONCAT3(BOP_TAGGED_UNION_NAME, _, name) name;
#define BOP_TAGGED_UNION__UNION_NAME CONCAT(BOP_TAGGED_UNION_NAME, __union)
union BOP_TAGGED_UNION__UNION_NAME {
    BOP_TAGGED_UNION_VARIANTS
};
#undef VARIANT

// Define the tags
#define VARIANT(name, contents) CONCAT3(BOP_TAGGED_UNION_NAME, __tag_, name),
#define BOP_TAGGED_UNION__ENUM_NAME CONCAT(BOP_TAGGED_UNION_NAME, __tag)
enum BOP_TAGGED_UNION__ENUM_NAME {
    BOP_TAGGED_UNION_VARIANTS
};
#undef VARIANT

// Define the tagged union
struct BOP_TAGGED_UNION_NAME {
    enum BOP_TAGGED_UNION__ENUM_NAME tag;
    union BOP_TAGGED_UNION__UNION_NAME value;
};

#endif

// Define matcher functions
#ifdef BOP_TAGGED_UNION_FUNCTION_DEFINITIONS
#define VARIANT(name, contents) \
    bool CONCAT3(BOP_TAGGED_UNION_NAME, _is_, name)(struct BOP_TAGGED_UNION_NAME u, struct CONCAT3(BOP_TAGGED_UNION_NAME, _, name) *out) { \
        if (u.tag == CONCAT3(BOP_TAGGED_UNION_NAME, __tag_, name)) { \
            if (out != NULL) { \
                *out = u.value.name; \
            } \
            return true; \
        } \
        return false; \
    }
#else
#define VARIANT(name, contents) \
    bool CONCAT3(BOP_TAGGED_UNION_NAME, _is_, name)(struct BOP_TAGGED_UNION_NAME u, struct CONCAT3(BOP_TAGGED_UNION_NAME, _, name) *out);
#endif
BOP_TAGGED_UNION_VARIANTS
#undef VARIANT

// Define builder functions
#ifdef BOP_TAGGED_UNION_FUNCTION_DEFINITIONS
#define VARIANT(name, contents) \
    struct BOP_TAGGED_UNION_NAME CONCAT3(BOP_TAGGED_UNION_NAME, _new_, name)(struct CONCAT3(BOP_TAGGED_UNION_NAME, _, name) value) { \
        return (struct BOP_TAGGED_UNION_NAME){ \
            .tag = CONCAT3(BOP_TAGGED_UNION_NAME, __tag_, name), \
            .value = (union BOP_TAGGED_UNION__UNION_NAME){ \
                .name = value \
            } \
        }; \
    }
#else
#define VARIANT(name, contents) \
    struct BOP_TAGGED_UNION_NAME CONCAT3(BOP_TAGGED_UNION_NAME, _new_, name)(struct CONCAT3(BOP_TAGGED_UNION_NAME, _, name) value);
#endif
BOP_TAGGED_UNION_VARIANTS
#undef VARIANT
