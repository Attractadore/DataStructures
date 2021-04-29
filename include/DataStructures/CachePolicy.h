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
#define CACHE_POLICY_UPPER_IDENTIFIER(name) CONCAT(CACHE_POLICY_UPPER_NAME, name)
#define CACHE_POLICY_LOWER_IDENTIFIER(name) CONCAT(CACHE_POLICY_LOWER_NAME, name)

typedef struct CACHE_POLICY_UPPER_IDENTIFIER(_T) CACHE_POLICY_NAME;

static inline CACHE_POLICY_NAME* CACHE_POLICY_LOWER_IDENTIFIER(Alloc)(size_t capacity, char const* algorithm) {
    return (CACHE_POLICY_NAME*) baseCachePolicyAlloc(capacity, sizeof(CACHE_POLICY_TYPE), algorithm);
}

static inline void CACHE_POLICY_LOWER_IDENTIFIER(Free)(CACHE_POLICY_NAME* const cache_policy) {
    baseCachePolicyFree((BaseCachePolicy*) cache_policy);
}

static inline bool CACHE_POLICY_LOWER_IDENTIFIER(Contains)(CACHE_POLICY_NAME const* const cache_policy, CACHE_POLICY_TYPE const key) {
    return baseCachePolicyContains((BaseCachePolicy const*) cache_policy, &key);
}

static inline CachePolicyAddResult CACHE_POLICY_LOWER_IDENTIFIER(Add)(CACHE_POLICY_NAME* const cache_policy, CACHE_POLICY_TYPE const key, CACHE_POLICY_TYPE* const result_ptr) {
    return baseCachePolicyAdd((BaseCachePolicy*) cache_policy, &key, result_ptr);
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
