#include <stdbool.h>
#include <stddef.h>

#define _CONCAT(X, Y) X ## Y
#define CONCAT(X, Y) _CONCAT(X, Y)
#define CONCAT3(X, Y, Z) CONCAT(X, CONCAT(Y, Z))

// Forward-define the main struct
struct TAGGED_UNION_NAME;

// Define the structs
#define VARIANT(name, contents) struct CONCAT3(TAGGED_UNION_NAME, _, name) contents;
TAGGED_UNION_VARIANTS
#undef VARIANT

// Define the union
#define VARIANT(name, contents) struct CONCAT3(TAGGED_UNION_NAME, _, name) name;
#define TAGGED_UNION__UNION_NAME CONCAT(TAGGED_UNION_NAME, __union)
union TAGGED_UNION__UNION_NAME {
    TAGGED_UNION_VARIANTS
};
#undef VARIANT

// Define the tags
#define VARIANT(name, contents) CONCAT3(TAGGED_UNION_NAME, __tag_, name),
#define TAGGED_UNION__ENUM_NAME CONCAT(TAGGED_UNION_NAME, __tag)
enum TAGGED_UNION__ENUM_NAME {
    TAGGED_UNION_VARIANTS
};
#undef VARIANT

// Define the tagged union
struct TAGGED_UNION_NAME {
    enum TAGGED_UNION__ENUM_NAME tag;
    union TAGGED_UNION__UNION_NAME value;
};

// Define matcher functions
#define VARIANT(name, contents) \
    bool CONCAT3(TAGGED_UNION_NAME, _is_, name)(struct TAGGED_UNION_NAME u, struct CONCAT3(TAGGED_UNION_NAME, _, name) *out) { \
        if (u.tag == CONCAT3(TAGGED_UNION_NAME, __tag_, name)) { \
            if (out != NULL) { \
                *out = u.value.name; \
            } \
            return true; \
        } \
        return false; \
    }
TAGGED_UNION_VARIANTS
#undef VARIANT
