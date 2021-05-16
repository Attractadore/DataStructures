#include "LRUCache.h"
#include "BaseDoubleList.h"
#include "BaseOpenHashTable.h"

#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

struct LRUCache_T {
  BaseOHT *table;
  DoubleList *list;
  size_t capacity;
};
/*------------------------------------------------------------------------------------------------------------------------------*/
LRUCache *lruCacheAlloc(size_t capacity, size_t element_size,
                        size_t element_align, BaseHashFunc hash_func,
                        BaseCompareFunc compare_func) {
  LRUCache *LRU = calloc(1, sizeof(*LRU));
  if (!LRU)
    return NULL;

  LRU->capacity = capacity;
  LRU->table =
      baseOHTInit(element_size, element_align, sizeof(DoubleListNode *),
                  alignof(DoubleListNode *), hash_func, compare_func);
  if (!LRU->table) {
    lruCacheFree(LRU);
    return NULL;
  }
  LRU->list = doubleListAlloc(element_size);
  if (!LRU->list) {
    lruCacheFree(LRU);
    return NULL;
  }
  return LRU;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lruCacheContains(LRUCache const *LRU, void const *key) {
  return baseOHTFind(LRU->table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lruCacheAddorReplace(LRUCache *LRU, void const *key,
                                          void *replace) {
  DoubleListNode *const *const node_ptr = baseOHTFind(LRU->table, key);
  if (node_ptr) {
    doubleListMoveToFront(LRU->list, *node_ptr);
    return CACHE_POLICY_ADD_NO_REPLACE;
  }

  if (LRU->capacity == doubleListSize(LRU->list)) {
    DoubleListNode const *old_end = doubleListConstBack(LRU->list);
    void const *old_data = doubleListNodeConstData(old_end);
    memcpy(replace, old_data, doubleListItemSize(LRU->list));
    baseOHTDelete(LRU->table, old_data);

    doubleListPopBack(LRU->list);

    if (lruCacheAdd(LRU, key) == CACHE_POLICY_ADD_ERROR) {
      return CACHE_POLICY_ADD_ERROR;
    }

    return CACHE_POLICY_ADD_REPLACE;
  }

  return lruCacheAdd(LRU, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lruCacheAdd(LRUCache *LRU, void const *key) {
  DoubleListNode *const front = doubleListAddFront(LRU->list, key);
  if (!front) {
    return CACHE_POLICY_ADD_ERROR;
  }

  if (!baseOHTInsert(LRU->table, key, &front)) {
    doubleListPopFront(LRU->list);
    return CACHE_POLICY_ADD_ERROR;
  }

  return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lruCacheFree(LRUCache *LRU) {
  if (LRU) {
    baseOHTFree(LRU->table);
    doubleListFree(LRU->list);

    free(LRU);
  }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
