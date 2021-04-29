#pragma once

#include "CachePolicyAddResult.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct BaseCachePolicy_T BaseCachePolicy;

BaseCachePolicy* baseCachePolicyAlloc(size_t capacity, size_t key_size, char const* algorithm);
void baseCachePolicyFree(BaseCachePolicy* cache_policy);

bool baseCachePolicyContains(BaseCachePolicy const* cache_policy, void const* key);

CachePolicyAddResult baseCachePolicyAdd(BaseCachePolicy const* cache_policy, void const* key, void* replace);
