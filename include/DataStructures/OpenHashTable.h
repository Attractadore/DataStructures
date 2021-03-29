#ifndef OHT_KEY_TYPE
#error Define the key type to use as OHT_KEY_TYPE
#endif

#ifndef OHT_KEY_TYPE_NAME
#error Define the name of the key type to use as OHT_KEY_TYPE_NAME
#endif

#ifndef OHT_VALUE_TYPE
#error Define the value type to use as OHT_VALUE_TYPE
#endif

#ifndef OHT_VALUE_TYPE_NAME
#error Define the name of the value type to use as OHT_VALUE_TYPE_NAME
#endif

#include "BaseOpenHashTable.h"

#define CONCAT_HELPER(a, b) a##b

#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define OHT_UPPER_NAME CONCAT(CONCAT(OHT, OHT_KEY_TYPE_NAME), OHT_VALUE_TYPE_NAME)
#define OHT_LOWER_NAME CONCAT(CONCAT(oht, OHT_KEY_TYPE_NAME), OHT_VALUE_TYPE_NAME)
#define OHT_NAME OHT_UPPER_NAME
#define OHT_UPPER_IDENTIFIER(name) CONCAT(OHT_UPPER_NAME, name)
#define OHT_LOWER_IDENTIFIER(name) CONCAT(OHT_LOWER_NAME, name)

typedef struct OHT_UPPER_IDENTIFIER(_T) OHT_NAME;

#define OHT_HASH_FUNC OHT_UPPER_IDENTIFIER(HashFunc)
#define OHT_COMPARE_FUNC OHT_UPPER_IDENTIFIER(CompareFunc)

typedef uint64_t (*OHT_HASH_FUNC)(OHT_KEY_TYPE const*);
typedef bool (*OHT_COMPARE_FUNC)(OHT_KEY_TYPE const*, OHT_KEY_TYPE const*);

typedef struct OHT_UPPER_IDENTIFIER(_T) OHT_NAME;

static inline OHT_NAME* OHT_LOWER_IDENTIFIER(Alloc)(const OHT_HASH_FUNC hash_func, const OHT_COMPARE_FUNC compare_func) {
    return (OHT_NAME*) baseOHTInit(sizeof(OHT_KEY_TYPE), _Alignof(OHT_KEY_TYPE), sizeof(OHT_VALUE_TYPE), _Alignof(OHT_VALUE_TYPE), (BaseHashFunc) hash_func, (BaseCompareFunc) compare_func);
}

static inline void OHT_LOWER_IDENTIFIER(Free)(OHT_NAME* const table) {
    baseOHTFree((BaseOHT*) table);
}

static inline OHT_VALUE_TYPE* OHT_LOWER_IDENTIFIER(Insert)(OHT_NAME* const table, OHT_KEY_TYPE const key, OHT_VALUE_TYPE const value) {
    return baseOHTInsert((BaseOHT*) table, &key, &value);
}

static inline OHT_VALUE_TYPE* OHT_LOWER_IDENTIFIER(Find)(OHT_NAME* const table, OHT_KEY_TYPE const key) {
    return baseOHTFind((BaseOHT*) table, &key);
}

static inline OHT_VALUE_TYPE* OHT_LOWER_IDENTIFIER(FindOrInsert)(OHT_NAME* const table, OHT_KEY_TYPE const key, OHT_VALUE_TYPE const value) {
    return baseOHTFindOrInsert((BaseOHT*) table, &key, &value);
}

static inline void OHT_LOWER_IDENTIFIER(Delete)(OHT_NAME* const table, OHT_KEY_TYPE const key) {
    baseOHTDelete((BaseOHT*) table, &key);
}

static inline bool OHT_LOWER_IDENTIFIER(IsEmpty)(OHT_NAME const* table) {
    return baseOHTIsEmpty((BaseOHT*) table);
}

static inline size_t OHT_LOWER_IDENTIFIER(Size)(OHT_NAME const* table) {
    return baseOHTSize((BaseOHT*) table);
}

#undef OHT_UPPER_IDENTIFIER
#undef OHT_LOWER_IDENTIFIER
#undef OHT_NAME
#undef OHT_UPPER_NAME
#undef OHT_LOWER_NAME

#undef CONCAT
#undef CONCAT_HELPER

#undef OHT_KEY_TYPE
#undef OHT_KEY_TYPE_NAME
#undef OHT_VALUE_TYPE
#undef OHT_VALUE_TYPE_NAME
