// This is an internal implementation header -- use CachePolicy.h instead
#pragma once

#include "BaseFunc.h"
#include "CachePolicyAddResult.h"

#include <stddef.h>

typedef struct BaseCachePolicy_T BaseCachePolicy;

typedef enum {
    CACHE_ALGORITHM_DUMMY,
    CACHE_ALGORITHM_INVALID,
} CacheAlgorithm;

extern char const* const cacheAlgorithmNames[CACHE_ALGORITHM_INVALID];

CacheAlgorithm getCacheAlgorithm(char const* algorithm_str);

BaseCachePolicy* baseCachePolicyAlloc(size_t capacity, size_t key_size, size_t key_align, BaseHashFunc hash_func, BaseCompareFunc compare_func, CacheAlgorithm algorithm);
void baseCachePolicyFree(BaseCachePolicy* cache_policy);

bool baseCachePolicyContains(BaseCachePolicy const* cache_policy, void const* key);

CachePolicyAddResult baseCachePolicyAdd(BaseCachePolicy const* cache_policy, void const* key, void* replace);
