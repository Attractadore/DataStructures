#pragma once
#include "CachePolicyAddResult"

typedef struct LRUCache_T LRUCache;

// LRUCacheAlloc - create LRUCache structure with hash_table, double_list,retuns pointer on structure
LRUCache* lruCacheAlloc(size_t  capacity, size_t element_size, size_t element_align, BaseHashFunc hash_func, BaseCompareFunc compare_func);// +
// lruCacheFree - free LRU
void lruCacheFree(LRUCache* LRU); // +

// lruCacheAddorReplace - adds key into cash and tells wich one remove from cache
CachePolicyAddResult lruCacheAddorReplace(LRUCache* LRU, void const* key, void const* replace)
// lruCacheAdd - adds key into cache
CachePolicyAddResult lruCacheAdd(LRUCache* LRU, void const* key); //
// lruCacheContains - check if the key should added to cache, returns 1 if yes
bool lruCacheContains(LRUCache* LRU, void const* key); // +- 
