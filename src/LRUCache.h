#pragma once

typedef struct LRUCache_T LRUCache;

// LRUCacheAlloc - create LRUCache structure with hash_table, vector, double_list
LRUCache* LRUCacheAllocDList(size_t element_size, size_t element_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func);// +-

// LRUsearchInDoubleList - search element in cache, returns its pointer or NULL
DoubleListNode* LRUsearchInDoubleList(LRUCache* LRU, void const* key);// +

// LRUCache - LRU algorithm of cache 
void LRUCache(size_t element_size, size_t element_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func)

// LRUstep - step of LRU algorithm
void LRUstep(LRUCache* LRU); // +-

// LRUgetNextData - returns next element from LRU->vector
void const* LRUgetNextData(LRUCache* LRU); // +-