#pragma once
#include "CachePolicyAddResult"

typedef struct LRUCache_T LRUCache;

// LRUCacheAlloc - create LRUCache structure with hash_table, vector, double_list,retuns pointer on structure
LRUCache* lruCacheAllocDList(size_t element_size, size_t element_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func);// +-
// lruCacheFree - free LRU
void lruCacheFree(LRUCache* LRU);

// lruCacheAdd - adds key into cash and tells wich one remove from cache, returns ...
CachePolicyAddResult lruCacheAdd(LRUCache* LRU, void const* key, void const* replace);
// lruCacheContains - check if the key should added to cache, returns 1 if yes
bool lruCacheContains(LRUCache* LRU, void const* key);
