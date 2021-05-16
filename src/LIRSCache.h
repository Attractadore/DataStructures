// LIRSCache.h

#pragma once

#include "BaseFunc.h"
#include "CachePolicyAddResult.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct LIRSCache_T LIRSCache;

// lirsCacheAlloc - create LIRSCache structure with hash_table, mono_list,
// capacity
LIRSCache *lirsCacheAlloc(size_t capacity, size_t element_size,
                          size_t element_align, BaseHashFunc hash_func,
                          BaseCompareFunc compare_func);
// lirsCacheFree - free LIRS
void lirsCacheFree(LIRSCache *LIRS);

// lirsCacheAddorReplace - adds key into cash and tells wich one was removed
// from cache
CachePolicyAddResult lirsCacheAddOrReplace(LIRSCache *LIRS, void const *key,
                                           void *replace);
// lirsCacheAdd - adds key into cache and adds ins status
CachePolicyAddResult lirsCacheAdd(LIRSCache *LIRS, void const *key, int status);
// lirsCacheContains - check if the key should added to cache, returns 1 if yes
bool lirsCacheContains(LIRSCache const *LIRS, void const *key);