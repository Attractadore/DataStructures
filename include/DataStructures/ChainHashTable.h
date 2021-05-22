/*
 * This describes a hash table based on chaining
 */

// CHT_KEY_TYPE and CHT_VALUE_TYPE define the type of the key-value pair of a
// hash table. CHT_KEY_TYPE_NAME and CHT_VALUE_TYPE_NAME define the name suffix
// of a hash table. If CHT_KEY_TYPE_NAME = Str and CHT_VALUE_TYPE_NAME = Int,
// CHT becomes CHTStrInt, chtAlloc becomes chtStrIntAlloc.

#ifndef CHT_KEY_TYPE
#error Define the key type to use as CHT_KEY_TYPE
#endif

#ifndef CHT_KEY_TYPE_NAME
#error Define the name of the key type to use as CHT_KEY_TYPE_NAME
#endif

#ifndef CHT_VALUE_TYPE
#error Define the value type to use as CHT_VALUE_TYPE
#endif

#ifndef CHT_VALUE_TYPE_NAME
#error Define the name of the value type to use as CHT_VALUE_TYPE_NAME
#endif

#include "BaseChainHashTable.h"

#define CONCAT_HELPER(a, b) a##b

#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define CHT_UPPER_NAME                                                         \
  CONCAT(CONCAT(CHT, CHT_KEY_TYPE_NAME), CHT_VALUE_TYPE_NAME)
#define CHT_LOWER_NAME                                                         \
  CONCAT(CONCAT(cht, CHT_KEY_TYPE_NAME), CHT_VALUE_TYPE_NAME)
#define CHT_NAME CHT_UPPER_NAME
#define CHT_UPPER_IDENTIFIER(name) CONCAT(CHT_UPPER_NAME, name)
#define CHT_LOWER_IDENTIFIER(name) CONCAT(CHT_LOWER_NAME, name)

typedef struct CHT_UPPER_IDENTIFIER(_T) CHT_NAME;

#define CHT_HASH_FUNC CHT_UPPER_IDENTIFIER(HashFunc)
#define CHT_COMPARE_FUNC CHT_UPPER_IDENTIFIER(CompareFunc)

typedef uint64_t (*CHT_HASH_FUNC)(CHT_KEY_TYPE const *);
typedef bool (*CHT_COMPARE_FUNC)(CHT_KEY_TYPE const *, CHT_KEY_TYPE const *);

typedef struct CHT_UPPER_IDENTIFIER(_T) CHT_NAME;

// chtAlloc: Create a new hash table.
static inline CHT_NAME *
CHT_LOWER_IDENTIFIER(Alloc)(const CHT_HASH_FUNC hash_func,
                            const CHT_COMPARE_FUNC compare_func) {
  return (CHT_NAME *)baseCHTInit(
      sizeof(CHT_KEY_TYPE), _Alignof(CHT_KEY_TYPE), sizeof(CHT_VALUE_TYPE),
      _Alignof(CHT_VALUE_TYPE), (BaseHashFunc)hash_func,
      (BaseCompareFunc)compare_func);
}

// chtFree: Free a hash table.
static inline void CHT_LOWER_IDENTIFIER(Free)(CHT_NAME *const table) {
  baseCHTFree((BaseCHT *)table);
}

// chtInsert: Insert key-value pair into a hash table.
// If the key is already preset, the old value is replaced by the new one.
// If an error occurs during insertion, NULL is returned.
// Otherwise, a pointer to the key's value is retured.
static inline CHT_VALUE_TYPE *
CHT_LOWER_IDENTIFIER(Insert)(CHT_NAME *const table, CHT_KEY_TYPE const key,
                             CHT_VALUE_TYPE const value) {
  return baseCHTInsert((BaseCHT *)table, &key, &value);
}

// chtFind: Find a key in a hash table.
// If the key is not found, NULL is returned.
// If the key is found, a pointer to its value is returned.
static inline CHT_VALUE_TYPE *
CHT_LOWER_IDENTIFIER(Find)(CHT_NAME *const table, CHT_KEY_TYPE const key) {
  return baseCHTFind((BaseCHT *)table, &key);
}

// chtFindOrInsert: Insert key-value pair into a hash table
// only if the key is not already present in the table.
// If an error occurs during insertion, NULL is returned.
// Otherwise, a pointer to the key's value is returned.
static inline CHT_VALUE_TYPE *CHT_LOWER_IDENTIFIER(FindOrInsert)(
    CHT_NAME *const table, CHT_KEY_TYPE const key, CHT_VALUE_TYPE const value) {
  return baseCHTFindOrInsert((BaseCHT *)table, &key, &value);
}

// chtDelete: Delete a key from a hash table.
static inline void CHT_LOWER_IDENTIFIER(Delete)(CHT_NAME *const table,
                                                CHT_KEY_TYPE const key) {
  baseCHTDelete((BaseCHT *)table, &key);
}

// chtIsEmpty: Check if a hash table is empty.
static inline bool CHT_LOWER_IDENTIFIER(IsEmpty)(CHT_NAME const *table) {
  return baseCHTIsEmpty((BaseCHT *)table);
}

// chtSize: Get the number of elements in a hash table.
static inline size_t CHT_LOWER_IDENTIFIER(Size)(CHT_NAME const *table) {
  return baseCHTSize((BaseCHT *)table);
}

#undef CHT_UPPER_IDENTIFIER
#undef CHT_LOWER_IDENTIFIER
#undef CHT_NAME
#undef CHT_UPPER_NAME
#undef CHT_LOWER_NAME

#undef CONCAT
#undef CONCAT_HELPER

#undef CHT_KEY_TYPE
#undef CHT_KEY_TYPE_NAME
#undef CHT_VALUE_TYPE
#undef CHT_VALUE_TYPE_NAME
