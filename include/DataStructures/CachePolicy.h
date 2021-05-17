/*
 * This describes a common interface to all other cache replacement algorithms
 */

// CACHE_POLICY_TYPE defines the type of the key you want to use in a
// CachePolicy. CACHE_POLICY_TYPE_NAME defines the name suffix of a CachePolicy.
// If CACHE_POLICY_TYPE_NAME = Int,
// CachePolicy becomes CachePolicyInt, cachePolicyAlloc becomes
// cachePolicyIntAlloc.

#ifndef CACHE_POLICY_TYPE
#error Define the type to use as CACHE_POLICY_TYPE
#endif

#ifndef CACHE_POLICY_TYPE_NAME
#error Define the name of the type to use as CACHE_POLICY_TYPE_NAME
#endif

#include "BaseCachePolicy.h"

#define CONCAT_HELPER(a, b) a##b

#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define CACHE_POLICY_UPPER_NAME CONCAT(CachePolicy, CACHE_POLICY_TYPE_NAME)
#define CACHE_POLICY_LOWER_NAME CONCAT(cachePolicy, CACHE_POLICY_TYPE_NAME)
#define CACHE_POLICY_NAME CACHE_POLICY_UPPER_NAME
#define CACHE_POLICY_UPPER_IDENTIFIER(name)                                    \
  CONCAT(CACHE_POLICY_UPPER_NAME, name)
#define CACHE_POLICY_LOWER_IDENTIFIER(name)                                    \
  CONCAT(CACHE_POLICY_LOWER_NAME, name)

#define CACHE_POLICY_HASH_FUNC CACHE_POLICY_UPPER_IDENTIFIER(HashFunc)
#define CACHE_POLICY_COMPARE_FUNC CACHE_POLICY_UPPER_IDENTIFIER(CompareFunc)
typedef uint64_t (*CACHE_POLICY_HASH_FUNC)(CACHE_POLICY_TYPE const *);
typedef bool (*CACHE_POLICY_COMPARE_FUNC)(CACHE_POLICY_TYPE const *,
                                          CACHE_POLICY_TYPE const *);

typedef struct CACHE_POLICY_UPPER_IDENTIFIER(_T) CACHE_POLICY_NAME;

// cachePolicyAlloc: Allocate a new cache policy manager.
static inline CACHE_POLICY_NAME *CACHE_POLICY_LOWER_IDENTIFIER(Alloc)(
    size_t capacity, const CACHE_POLICY_HASH_FUNC hash_func,
    const CACHE_POLICY_COMPARE_FUNC compare_func,
    const CacheAlgorithm algorithm) {
  return (CACHE_POLICY_NAME *)baseCachePolicyAlloc(
      capacity, sizeof(CACHE_POLICY_TYPE), _Alignof(CACHE_POLICY_TYPE),
      (BaseHashFunc)hash_func, (BaseCompareFunc)compare_func, algorithm);
}

// cachePolicyFree: Free a cache policy manager.
static inline void
CACHE_POLICY_LOWER_IDENTIFIER(Free)(CACHE_POLICY_NAME *const cache_policy) {
  baseCachePolicyFree((BaseCachePolicy *)cache_policy);
}

// cachePolicyContains: Check if a key should be currently cached.
static inline bool CACHE_POLICY_LOWER_IDENTIFIER(Contains)(
    CACHE_POLICY_NAME const *const cache_policy, CACHE_POLICY_TYPE const key) {
  return baseCachePolicyContains((BaseCachePolicy const *)cache_policy, &key);
}

// cachePolicyAdd: Add a key to the set of those that should be cached.
// If an error occurs, CACHE_POLICY_ADD_ERROR is returned.
// If no key should be removed from the cache, CACHE_POLICY_ADD_NO_REPLACE is
// returned. If a key should be removed from the cache, CACHE_POLICY_ADD_REPLACE
// is returned and the key that should be removed is written to replace.
static inline CachePolicyAddResult
CACHE_POLICY_LOWER_IDENTIFIER(Add)(CACHE_POLICY_NAME *const cache_policy,
                                   CACHE_POLICY_TYPE const key,
                                   CACHE_POLICY_TYPE *const result_ptr) {
  return baseCachePolicyAdd((BaseCachePolicy *)cache_policy, &key, result_ptr);
}

#undef CACHE_POLICY_UPPER_IDENTIFIER
#undef CACHE_POLICY_LOWER_IDENTIFIER
#undef CACHE_POLICY_NAME
#undef CACHE_POLICY_UPPER_NAME
#undef CACHE_POLICY_LOWER_NAME

#undef CONCAT
#undef CONCAT_HELPER

#undef CACHE_POLICY_TYPE_NAME
#undef CACHE_POLICY_TYPE
