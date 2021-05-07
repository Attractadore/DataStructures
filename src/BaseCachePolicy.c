#include "BaseCachePolicy.h"
#include "DummyCache.h"
#include "LRUCache.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef CachePolicyAddResult (*BaseCacheAddFunc)(void*, void const*, void*);
typedef bool (*BaseCacheContainsFunc)(void const*, void const*);
typedef void (*BaseCacheFreeFunc)(void*);

struct BaseCachePolicy_T {
    void* cache;
    BaseCacheAddFunc add_func;
    BaseCacheContainsFunc contains_func;
    BaseCacheFreeFunc free_func;
};

char const* const cacheAlgorithmNames[CACHE_ALGORITHM_INVALID] =
    {
        [CACHE_ALGORITHM_DUMMY] = "Dummy",
        [CACHE_ALGORITHM_LRU] = "LRU",
};

CacheAlgorithm getCacheAlgorithm(char const* const algorithm_str) {
    assert(algorithm_str);

    for (size_t i = 0; i < CACHE_ALGORITHM_INVALID; i++) {
        if (strcmp(algorithm_str, cacheAlgorithmNames[i]) == 0) {
            return i;
        }
    }

    return CACHE_ALGORITHM_INVALID;
}

BaseCachePolicy* baseCachePolicyAlloc(const size_t capacity, const size_t key_size, const BaseHashFunc hash_func, const BaseCompareFunc compare_func, const CacheAlgorithm algorithm) {
    assert(capacity && key_size && hash_func && compare_func && algorithm);

    BaseCachePolicy* const cache_policy = calloc(1, sizeof(*cache_policy));
    if (!cache_policy) {
        return NULL;
    }

    switch (algorithm) {
        case CACHE_ALGORITHM_DUMMY:
            cache_policy->cache = dummyCacheAlloc(capacity, key_size);
            if (!cache_policy->cache) {
                free(cache_policy);
                return NULL;
            }
            cache_policy->add_func = (BaseCacheAddFunc) dummyCacheAdd;
            cache_policy->contains_func = (BaseCacheContainsFunc) dummyCacheContains;
            cache_policy->free_func = (BaseCacheFreeFunc) dummyCacheFree;
            return cache_policy;
        case CACHE_ALGORITHM_LRU:
            cache_policy->cache = lruCacheAlloc(capacity, key_size, hash_func, compare_func);
            if (!cache_policy->cache) {
                free(cache_policy);
                return NULL;
            }
            cache_policy->add_func = (BaseCacheAddFunc)lruCacheAddorReplace;
            cache_policy->contains_func = (BaseCacheContainsFunc)lruCacheContains;
            cache_policy->free_func = (BaseCacheFreeFunc)lruCacheFree;
            return cache_policy;
        case CACHE_ALGORITHM_INVALID:
            return NULL;
    }

    assert(!"Invalid enum value");
}

void baseCachePolicyFree(BaseCachePolicy* const cache_policy) {
    if (!cache_policy) {
        return;
    }

    cache_policy->free_func(cache_policy->cache);
    free(cache_policy);
}

bool baseCachePolicyContains(BaseCachePolicy const* const cache_policy, void const* const key) {
    assert(cache_policy && key);

    return cache_policy->contains_func(cache_policy->cache, key);
}

CachePolicyAddResult baseCachePolicyAdd(BaseCachePolicy const* const cache_policy, void const* const key, void* const replace) {
    assert(cache_policy && key && replace);

    return cache_policy->add_func(cache_policy->cache, key, replace);
}
