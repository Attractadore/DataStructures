#include "LIRSCache.h"
#include "BaseMonoList.h"
#include "BaseOpenHashTable.h"

#include <assert.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

struct LIRSCache_T {
    MonoList* queue_list;
    BaseOHT* queue_table;

    MonoList* hir_list;
    BaseOHT* hir_table;

    size_t LIR_capacity;
    size_t HIR_capacity;
};

static void setToLIR(uintptr_t* const data) {
    *data = *data | 1;
}

static bool isLIR(const uintptr_t data) {
    return data & 1;
}

static MonoListNode* getPointer(const uintptr_t data) {
    return (MonoListNode*) (data & (UINTPTR_MAX - 1));
}

static void setPointer(uintptr_t* const data, MonoListNode const* node) {
    uintptr_t data_status = isLIR(*data);
    *data = (uintptr_t) node;
    *data = *data ^ data_status;
}

static CachePolicyAddResult lirsCacheAddAsLIR(LIRSCache* LIRS, void const* key);
static CachePolicyAddResult lirsCacheAddToQueue(LIRSCache* LIRS, void const* key);
static CachePolicyAddResult lirsCacheAddToHIR(LIRSCache* LIRS, void const* key);

static void lirsCacheDeleteFromQueue(LIRSCache* LIRS, void const* key);
static void lirsCacheDeleteFromHIR(LIRSCache* LIRS, void const* key);
static void lirsCachePopQueue(LIRSCache* LIRS);
static void lirsCachePopHIR(LIRSCache* LIRS, void* replace);

static void lirsCacheMoveToFrontQueue(LIRSCache* LIRS, uintptr_t* prev_ptr);
static void lirsCacheUpdateNextNext(LIRSCache* LIRS, MonoListNode* prev);

static MonoListNode* lirsCacheQueueOldest(LIRSCache* LIRS) {
    assert(LIRS);
    return monoListNodeNext(monoListFront(LIRS->queue_list));
}

static MonoListNode* lirsCacheQueueNewest(LIRSCache* LIRS) {
    assert(LIRS);
    return monoListBack(LIRS->queue_list);
}

static MonoListNode* lirsCacheHIROldest(LIRSCache* LIRS) {
    assert(LIRS);
    return monoListNodeNext(monoListFront(LIRS->hir_list));
}

static MonoListNode* lirsCacheHIRNewest(LIRSCache* LIRS) {
    assert(LIRS);
    return monoListBack(LIRS->hir_list);
}

#if 0
int main() {
    MonoList* list = monoListAlloc(sizeof(int));
    int a = 7;
    monoListAddToFront(list, &a);
    a = 2;
    monoListAddToFront(list, &a);
    MonoListNode* start = monoListFront();

    lirsCacheStatus2(start);
    lirsCacheChangeStatus2(list->start);

    return 0;
}
#endif
/*------------------------------------------------------------------------------------------------------------------------------*/
LIRSCache* lirsCacheAlloc(size_t capacity, size_t element_size, size_t element_align, BaseHashFunc hash_func, BaseCompareFunc compare_func) {
    assert(capacity >= 2);
    LIRSCache* LIRS = calloc(1, sizeof(*LIRS));
    if (!LIRS)
        return NULL;
    const double LIR_portion = 0.9;
    LIRS->LIR_capacity = LIR_portion * capacity;
    if (!LIRS->LIR_capacity) {
        LIRS->LIR_capacity = 1;
    }
    LIRS->HIR_capacity = capacity - LIRS->LIR_capacity;

    LIRS->queue_table = baseOHTInit(element_size, element_align, sizeof(uintptr_t), alignof(uintptr_t), hash_func, compare_func);
    if (LIRS->queue_table) {
        LIRS->hir_table = baseOHTInit(element_size, element_align, sizeof(MonoListNode*), alignof(MonoListNode*), hash_func, compare_func);
        if (LIRS->hir_table) {
            LIRS->queue_list = monoListAlloc(element_size);
            if (LIRS->queue_list) {
                void* const zeroes = calloc(1, element_size);
                if (zeroes) {
                    MonoListNode* queue_sentinel = monoListAddToFront(LIRS->queue_list, zeroes);
                    if (queue_sentinel) {
                        LIRS->hir_list = monoListAlloc(element_size);
                        if (LIRS->hir_list) {
                            MonoListNode* hir_sentinel = monoListAddToFront(LIRS->hir_list, zeroes);
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
void lirsCacheFree(LIRSCache* LIRS) {
    if (LIRS) {
        baseOHTFree(LIRS->queue_table);
        baseOHTFree(LIRS->hir_table);
        monoListFree(LIRS->queue_list);
        monoListFree(LIRS->hir_list);
        free(LIRS);
    }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lirsCacheContains(LIRSCache const* LIRS, void const* key) {
    uintptr_t const* const temp = baseOHTFind(LIRS->queue_table, key);

    if (temp == NULL)
        return false;
    uintptr_t data = *temp;

    if (isLIR(data))
        return true;

    return baseOHTFind(LIRS->hir_table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCacheMoveToFrontQueue(LIRSCache* LIRS, uintptr_t* value) {
    MonoListNode* prev = getPointer(*value);
    MonoListNode* cur = monoListNodeNext(prev);
    assert(cur);

    MonoListNode* const newest = lirsCacheQueueNewest(LIRS);
    if (cur == newest) {
        return;
    }

    lirsCacheUpdateNextNext(LIRS, prev);
    setPointer(value, newest);
    monoListMoveNextToBack(LIRS->queue_list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCachePrune(LIRSCache* LIRS) {
    while (true) {
        MonoListNode* const end = lirsCacheQueueOldest(LIRS);
        assert(end);
        uintptr_t* end_data = baseOHTFind(LIRS->queue_table, monoListConstNodeData(end));
        if (isLIR(*end_data))
            break;
        lirsCacheDeleteFromQueue(LIRS, monoListConstNodeData(end));
    }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddOrReplace(LIRSCache* LIRS, void const* key, void* replace) {
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
    //    If this block is present in the queue, move it to the front of the queue,
    //    promote it to a LIR block, demote the last block of the queue to a HIR block,
    //    and prune the queue.
    //    If it is not present in the queue, and it to the queue.
    uintptr_t* queue_value = baseOHTFind(LIRS->queue_table, key);
    if (queue_value && isLIR(*queue_value)) {
        lirsCacheMoveToFrontQueue(LIRS, queue_value);
        lirsCachePrune(LIRS);
        return CACHE_POLICY_ADD_NO_REPLACE;
    }
    MonoListNode** hir_value = baseOHTFind(LIRS->hir_table, key);
    CachePolicyAddResult res = CACHE_POLICY_ADD_NO_REPLACE;
    if (!hir_value) {
        if (monoListSize(LIRS->queue_list) <= LIRS->LIR_capacity) {
            return lirsCacheAddAsLIR(LIRS, key);
        }
        if (monoListSize(LIRS->hir_list) == LIRS->HIR_capacity + 1) {
            lirsCachePopHIR(LIRS, replace);
            res = CACHE_POLICY_ADD_REPLACE;
        }
        if (lirsCacheAddToHIR(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
            return CACHE_POLICY_ADD_ERROR;
        }
        hir_value = baseOHTFind(LIRS->hir_table, key);
        assert(hir_value);
    }

    if (!queue_value) {
        if (lirsCacheAddToQueue(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
            return CACHE_POLICY_ADD_ERROR;
        }
        return res;
    }

    lirsCacheMoveToFrontQueue(LIRS, queue_value);
    setToLIR(queue_value);
    void const* oldest_lir_key = monoListConstNodeData(lirsCacheQueueOldest(LIRS));
    if (lirsCacheAddToHIR(LIRS, oldest_lir_key) == CACHE_POLICY_ADD_ERROR) {
        return CACHE_POLICY_ADD_ERROR;
    }
    lirsCachePopQueue(LIRS);
    lirsCachePrune(LIRS);

    return res;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
// TODO: maybe replace should not be passed
static void lirsCachePopHIR(LIRSCache* LIRS, void* replace) {
    MonoListNode const* const oldest = lirsCacheHIROldest(LIRS);
    void const* const oldest_key = monoListConstNodeData(oldest);
    memcpy(replace, oldest_key, monoListItemSize(LIRS->hir_list));
    lirsCacheDeleteFromHIR(LIRS, oldest_key);
}

static void lirsCachePopQueue(LIRSCache* LIRS) {
    MonoListNode const* oldest = lirsCacheQueueOldest(LIRS);
    assert(oldest);
    lirsCacheDeleteFromQueue(LIRS, monoListConstNodeData(oldest));
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCacheDeleteFromQueue(LIRSCache* LIRS, void const* key) {
    uintptr_t* value = baseOHTFind(LIRS->queue_table, key);
    assert(value);
    MonoListNode* prev = getPointer(*value);

    lirsCacheUpdateNextNext(LIRS, prev);

    baseOHTDelete(LIRS->queue_table, key);
    monoListDeleteNext(LIRS->queue_list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lirsCacheUpdateNextNext(LIRSCache* LIRS, MonoListNode* prev) {
    MonoListNode* cur = monoListNodeNext(prev);
    assert(cur);
    MonoListNode* next = monoListNodeNext(cur);
    if (next) {
        uintptr_t* next_value = baseOHTFind(LIRS->queue_table, monoListConstNodeData(next));
        assert(next_value);
        setPointer(next_value, prev);
    }
}
static void lirsCacheDeleteFromHIR(LIRSCache* LIRS, void const* key) {
    MonoListNode** cur_value = baseOHTFind(LIRS->hir_table, key);
    assert(cur_value);
    MonoListNode* prev = *cur_value;
    // TODO: move this into a separate function
    // (maybe combine with lirsCacheUpdateNextNext?)
    MonoListNode* cur = monoListNodeNext(prev);
    assert(cur);
    MonoListNode* next = monoListNodeNext(cur);
    if (next) {
        MonoListNode** next_value = baseOHTFind(LIRS->hir_table, monoListConstNodeData(next));
        assert(next_value);
        *next_value = prev;
    }
    //
    baseOHTDelete(LIRS->hir_table, key);
    monoListDeleteNext(LIRS->hir_list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static CachePolicyAddResult lirsCacheAddToQueue(LIRSCache* LIRS, void const* key) {
    MonoListNode* newest = lirsCacheQueueNewest(LIRS);
    uintptr_t* value = baseOHTInsert(LIRS->queue_table, key, &newest);
    if (!value) {
        return CACHE_POLICY_ADD_ERROR;
    }

    MonoListNode* new = monoListAddToBack(LIRS->queue_list, key);
    if (!new) {
        baseOHTDelete(LIRS->queue_table, key);
        return CACHE_POLICY_ADD_ERROR;
    }

    return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static CachePolicyAddResult lirsCacheAddToHIR(LIRSCache* LIRS, void const* key) {
    MonoListNode* newest = lirsCacheHIRNewest(LIRS);
    MonoListNode* value = baseOHTInsert(LIRS->hir_table, key, &newest);
    if (!value) {
        return CACHE_POLICY_ADD_ERROR;
    }
    MonoListNode* new = monoListAddToBack(LIRS->hir_list, key);
    if (!new) {
        baseOHTDelete(LIRS->hir_table, key);
        return CACHE_POLICY_ADD_ERROR;
    }

    return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
static CachePolicyAddResult lirsCacheAddAsLIR(LIRSCache* LIRS, void const* key) {
    CachePolicyAddResult res = lirsCacheAddToQueue(LIRS, key);
    if (res != CACHE_POLICY_ADD_ERROR) {
        uintptr_t* value = baseOHTFind(LIRS->queue_table, key);
        assert(value);
        setToLIR(value);
    }
    return res;
}
