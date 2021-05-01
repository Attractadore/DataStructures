#pragma once

#include "BaseDoubleList.h"
#include "BaseOpenHashTable.h"
#include "BaseVector.h"

typedef struct LRUCache_T LRUCache;

// LRUCacheAlloc - create LRUCache structure with hash_table, vector, double_list
LRUCache* LRUCacheAlloc(size_t element_size, ...);// +-

// LRUsearch - search element in cache, returns its pointer or NULL
DoubleListNode* LRUsearchDoubleList(LRUCache* LRU, void const* key);// +

// LRU - ...
void LRU(void); // dont know what to send 

// LRUstep - step of LRU algorithm
void LRUstep(LRUCache* LRU); // +-

// LRUgetNextData - returns next element from LRU->vector
void const* LRUgetNextData(LRUCache* LRU); // +-