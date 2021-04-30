#pragma once

#include "BaseCompareFunc.h"
#include "BaseHashFunc.h"
#include "CachePolicyAddResult.h"

#include <stddef.h>

typedef struct BaseCachePolicy_T BaseCachePolicy;

BaseCachePolicy* baseCachePolicyAlloc(size_t capacity, size_t key_size, BaseHashFunc hash_func, BaseCompareFunc compare_func, char const* algorithm);
void baseCachePolicyFree(BaseCachePolicy* cache_policy);

bool baseCachePolicyContains(BaseCachePolicy const* cache_policy, void const* key);

CachePolicyAddResult baseCachePolicyAdd(BaseCachePolicy const* cache_policy, void const* key, void* replace);
