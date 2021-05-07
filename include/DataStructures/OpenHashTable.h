// OHT_KEY_TYPE and OHT_VALUE_TYPE define the type of the key-value pair of a hash table.
// OHT_KEY_TYPE_NAME and OHT_VALUE_TYPE_NAME define the name suffix of a hash table.
// If OHT_KEY_TYPE_NAME = Str and OHT_VALUE_TYPE_NAME = Int,
// OHT becomes OHTStrInt, ohtAlloc becomes ohtStrIntAlloc.

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

// ohtAlloc: Create a new hash table.
static inline OHT_NAME* OHT_LOWER_IDENTIFIER(Alloc)(const OHT_HASH_FUNC hash_func, const OHT_COMPARE_FUNC compare_func) {
    return (OHT_NAME*) baseOHTInit(sizeof(OHT_KEY_TYPE), _Alignof(OHT_KEY_TYPE), sizeof(OHT_VALUE_TYPE), _Alignof(OHT_VALUE_TYPE), (BaseHashFunc) hash_func, (BaseCompareFunc) compare_func);
}

// ohtFree: Free a hash table.
static inline void OHT_LOWER_IDENTIFIER(Free)(OHT_NAME* const table) {
    baseOHTFree((BaseOHT*) table);
}

// ohtInsert: Insert key-value pair into a hash table.
// If the key is already preset, the old value is replaced by the new one.
// If an error occurs during insertion, NULL is returned.
// Otherwise, a pointer to the key's value is retured.
static inline OHT_VALUE_TYPE* OHT_LOWER_IDENTIFIER(Insert)(OHT_NAME* const table, OHT_KEY_TYPE const key, OHT_VALUE_TYPE const value) {
    return baseOHTInsert((BaseOHT*) table, &key, &value);
}

// ohtFind: Find a key in a hash table.
// If the key is not found, NULL is returned.
// If the key is found, a pointer to its value is returned.
static inline OHT_VALUE_TYPE* OHT_LOWER_IDENTIFIER(Find)(OHT_NAME* const table, OHT_KEY_TYPE const key) {
    return baseOHTFind((BaseOHT*) table, &key);
}

// ohtFindOrInsert: Insert key-value pair into a hash table
// only if the key is not already present in the table.
// If an error occurs during insertion, NULL is returned.
// Otherwise, a pointer to the key's value is returned.
static inline OHT_VALUE_TYPE* OHT_LOWER_IDENTIFIER(FindOrInsert)(OHT_NAME* const table, OHT_KEY_TYPE const key, OHT_VALUE_TYPE const value) {
    return baseOHTFindOrInsert((BaseOHT*) table, &key, &value);
}

// ohtDelete: Delete a key from a hash table.
static inline void OHT_LOWER_IDENTIFIER(Delete)(OHT_NAME* const table, OHT_KEY_TYPE const key) {
    baseOHTDelete((BaseOHT*) table, &key);
}

// ohtIsEmpty: Check if a hash table is empty.
static inline bool OHT_LOWER_IDENTIFIER(IsEmpty)(OHT_NAME const* table) {
    return baseOHTIsEmpty((BaseOHT*) table);
}

// ohtSize: Get the number of elements in a hash table.
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
