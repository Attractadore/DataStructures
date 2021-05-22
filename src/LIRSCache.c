#include "LIRSCache.h"
#include "BaseChainHashTable.h"
#include "BaseMonoList.h"

#include <assert.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

struct LIRSCache_T {
  MonoList *queue_list;
  BaseCHT *queue_table;

  MonoList *hir_list;
  BaseCHT *hir_table;

  size_t LIR_capacity;
  size_t HIR_capacity;
};

static void setToLIR(uintptr_t *const data) { *data = *data | 1; }

static bool isLIR(const uintptr_t data) { return data & 1; }

static MonoListNode *getPointer(const uintptr_t data) {
  return (MonoListNode *)(data & (UINTPTR_MAX - 1));
}

static void setPointer(uintptr_t *const data, MonoListNode const *node) {
  uintptr_t data_status = isLIR(*data);
  *data = (uintptr_t)node;
  *data = *data | data_status;
}

static MonoListNode *lirsCacheNewest(MonoList *list) {
  assert(list);
  return monoListBack(list);
}

static MonoListNode *lirsCacheQueueOldest(LIRSCache *LIRS) {
  assert(LIRS);
  return monoListNodeNext(monoListFront(LIRS->queue_list));
}

static MonoListNode *lirsCacheHIROldest(LIRSCache *LIRS) {
  assert(LIRS);
  return monoListNodeNext(monoListFront(LIRS->hir_list));
}

static CachePolicyAddResult lirsCacheAddAsLIR(LIRSCache *LIRS, void const *key);
static CachePolicyAddResult lirsCacheAddTo(void const *key, BaseCHT *table,
                                           MonoList *list);
static CachePolicyAddResult lirsCacheAddToQueue(LIRSCache *LIRS,
                                                void const *key) {
  return lirsCacheAddTo(key, LIRS->queue_table, LIRS->queue_list);
}
static CachePolicyAddResult lirsCacheAddToHIR(LIRSCache *LIRS,
                                              void const *key) {
  return lirsCacheAddTo(key, LIRS->hir_table, LIRS->hir_list);
}

static void lirsCacheDeleteFrom(void const *key, BaseCHT *table,
                                MonoList *list);
static void lirsCacheDeleteFromQueue(LIRSCache *LIRS, void const *key) {
  lirsCacheDeleteFrom(key, LIRS->queue_table, LIRS->queue_list);
}
static void lirsCacheDeleteFromHIR(LIRSCache *LIRS, void const *key) {
  lirsCacheDeleteFrom(key, LIRS->hir_table, LIRS->hir_list);
}

static void lirsCachePopQueue(LIRSCache *LIRS) {
  lirsCacheDeleteFromQueue(LIRS,
                           monoListConstNodeData(lirsCacheQueueOldest(LIRS)));
}
static void lirsCachePopHIR(LIRSCache *LIRS) {
  lirsCacheDeleteFromHIR(LIRS, monoListConstNodeData(lirsCacheHIROldest(LIRS)));
}

static void lirsCacheMoveToFront(uintptr_t *value, BaseCHT *table,
                                 MonoList *list);
static void lirsCacheMoveToFrontQueue(LIRSCache *LIRS, uintptr_t *prev_ptr) {
  lirsCacheMoveToFront(prev_ptr, LIRS->queue_table, LIRS->queue_list);
}
static void lirsCacheMoveToFrontHIR(LIRSCache *LIRS, MonoListNode **value) {
  lirsCacheMoveToFront((uintptr_t *)value, LIRS->hir_table, LIRS->hir_list);
}
static void lirsCacheUpdateNextNext(MonoListNode *prev, BaseCHT *table);

/*------------------------------------------------------------------------------------------------------------------------------*/
LIRSCache *lirsCacheAlloc(size_t capacity, size_t element_size,
                          size_t element_align, BaseHashFunc hash_func,
                          BaseCompareFunc compare_func) {
  assert(capacity >= 2);
  LIRSCache *LIRS = calloc(1, sizeof(*LIRS));
  if (!LIRS)
    return NULL;
  const double LIR_portion = 0.99;
  LIRS->LIR_capacity = LIR_portion * capacity;
  if (!LIRS->LIR_capacity) {
    LIRS->LIR_capacity = 1;
  }
  LIRS->HIR_capacity = capacity - LIRS->LIR_capacity;

  LIRS->queue_table =
      baseCHTInit(element_size, element_align, sizeof(uintptr_t),
                  alignof(uintptr_t), hash_func, compare_func);
  if (LIRS->queue_table) {
    LIRS->hir_table =
        baseCHTInit(element_size, element_align, sizeof(MonoListNode *),
                    alignof(MonoListNode *), hash_func, compare_func);
    if (LIRS->hir_table) {
      LIRS->queue_list = monoListAlloc(element_size);
      if (LIRS->queue_list) {
        void *const zeroes = calloc(1, element_size);
        if (zeroes) {
          MonoListNode *queue_sentinel =
              monoListAddToFront(LIRS->queue_list, zeroes);
          if (queue_sentinel) {
            LIRS->hir_list = monoListAlloc(element_size);
            if (LIRS->hir_list) {
              MonoListNode *hir_sentinel =
                  monoListAddToFront(LIRS->hir_list, zeroes);
              if (hir_sentinel) {
                free(zeroes);
                return LIRS;
              }
            }
          }
        }
        free(zeroes);
      }
    }
  }

  lirsCacheFree(LIRS);
  return NULL;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheFree(LIRSCache *LIRS) {
  if (LIRS) {
    baseCHTFree(LIRS->queue_table);
    baseCHTFree(LIRS->hir_table);
    monoListFree(LIRS->queue_list);
    monoListFree(LIRS->hir_list);
    free(LIRS);
  }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lirsCacheContains(LIRSCache const *LIRS, void const *key) {
  uintptr_t const *const value = baseCHTFind(LIRS->queue_table, key);

  if (value && isLIR(*value)) {
    return true;
  }

  return baseCHTFind(LIRS->hir_table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCacheMoveToFront(uintptr_t *value, BaseCHT *table,
                                 MonoList *list) {
  MonoListNode *prev = getPointer(*value);
  MonoListNode *cur = monoListNodeNext(prev);
  assert(cur);

  MonoListNode *const newest = lirsCacheNewest(list);
  if (cur == newest) {
    return;
  }

  lirsCacheUpdateNextNext(prev, table);
  setPointer(value, newest);
  monoListMoveNextToBack(list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCachePrune(LIRSCache *LIRS) {
  while (true) {
    MonoListNode *const end = lirsCacheQueueOldest(LIRS);
    assert(end);
    uintptr_t *end_data =
        baseCHTFind(LIRS->queue_table, monoListConstNodeData(end));
    if (isLIR(*end_data))
      break;
    lirsCacheDeleteFromQueue(LIRS, monoListConstNodeData(end));
  }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddOrReplace(LIRSCache *LIRS, void const *key,
                                           void *replace) {
  // When adding a key to the cache, the following can happen:
  // 1) This is a resident LIR block.
  //    Move it to the front of the queue and prune the queue.
  // 2) This is a non-resident block.
  //    If the number of LIR blocks is less than the maximum amount,
  //    add it as a LIR block.
  //    If the number of HIR blocks is less the the maximum amount,
  //    add it as a HIR block and carry on.
  //    Otherwise, evict a HIR block, add the key as a HIR block and carry on.
  // 3) This is a resident HIR block.
  //    If this block is present in the queue, move it to the front of the
  //    queue, promote it to a LIR block, demote the last block of the queue to
  //    a HIR block, and prune the queue. If it is not present in the queue, and
  //    it to the queue.

  uintptr_t *queue_value = baseCHTFind(LIRS->queue_table, key);
  if (queue_value && isLIR(*queue_value)) {
    lirsCacheMoveToFrontQueue(LIRS, queue_value);
    lirsCachePrune(LIRS);
    return CACHE_POLICY_ADD_NO_REPLACE;
  }
  MonoListNode **hir_value = baseCHTFind(LIRS->hir_table, key);
  CachePolicyAddResult res = CACHE_POLICY_ADD_NO_REPLACE;
  if (!hir_value) {
    if (monoListSize(LIRS->queue_list) - 1 < LIRS->LIR_capacity) {
      return lirsCacheAddAsLIR(LIRS, key);
    }
    if (monoListSize(LIRS->hir_list) - 1 == LIRS->HIR_capacity) {
      memcpy(replace, monoListNodeData(lirsCacheHIROldest(LIRS)),
             monoListItemSize(LIRS->hir_list));
      lirsCachePopHIR(LIRS);
      res = CACHE_POLICY_ADD_REPLACE;
    }
    if (lirsCacheAddToHIR(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
      return CACHE_POLICY_ADD_ERROR;
    }
    hir_value = baseCHTFind(LIRS->hir_table, key);
    assert(hir_value);
  } else {
    lirsCacheMoveToFrontHIR(LIRS, hir_value);
  }

  if (!queue_value) {
    if (lirsCacheAddToQueue(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
      return CACHE_POLICY_ADD_ERROR;
    }
    return res;
  }

  lirsCacheMoveToFrontQueue(LIRS, queue_value);
  setToLIR(queue_value);
  void const *oldest_lir_key =
      monoListConstNodeData(lirsCacheQueueOldest(LIRS));
  if (lirsCacheAddToHIR(LIRS, oldest_lir_key) == CACHE_POLICY_ADD_ERROR) {
    return CACHE_POLICY_ADD_ERROR;
  }
  lirsCacheDeleteFromHIR(LIRS, key);
  lirsCachePopQueue(LIRS);
  lirsCachePrune(LIRS);

  return res;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCacheDeleteFrom(void const *key, BaseCHT *table,
                                MonoList *list) {
  uintptr_t *value = baseCHTFind(table, key);
  assert(value);
  MonoListNode *prev = getPointer(*value);

  lirsCacheUpdateNextNext(prev, table);
  baseCHTDelete(table, key);
  monoListDeleteNext(list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCacheUpdateNextNext(MonoListNode *prev, BaseCHT *table) {
  MonoListNode *cur = monoListNodeNext(prev);
  assert(cur);
  MonoListNode *next = monoListNodeNext(cur);
  if (next) {
    uintptr_t *next_value = baseCHTFind(table, monoListConstNodeData(next));
    assert(next_value);
    setPointer(next_value, prev);
  }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static CachePolicyAddResult lirsCacheAddTo(void const *key, BaseCHT *table,
                                           MonoList *list) {
  MonoListNode *newest = lirsCacheNewest(list);
  uintptr_t *value = baseCHTInsert(table, key, &newest);
  if (!value) {
    return CACHE_POLICY_ADD_ERROR;
  }

  MonoListNode *new = monoListAddToBack(list, key);
  if (!new) {
    baseCHTDelete(table, key);
    return CACHE_POLICY_ADD_ERROR;
  }

  return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static CachePolicyAddResult lirsCacheAddAsLIR(LIRSCache *LIRS,
                                              void const *key) {
  CachePolicyAddResult res = lirsCacheAddToQueue(LIRS, key);
  if (res != CACHE_POLICY_ADD_ERROR) {
    uintptr_t *value = baseCHTFind(LIRS->queue_table, key);
    assert(value);
    setToLIR(value);
  }
  return res;
}
