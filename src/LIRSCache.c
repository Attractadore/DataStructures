#include "LIRSCache.h"
#include "BaseMonoList.h"
#include "BaseOpenHashTable.h"

#include <assert.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

// lirsCacheGetPointer - cast data to list_pointer
MonoListNode* lirsCacheGetPointer(uintptr_t data);
// Set the pointer part of data to ptr
void lirsCacheSetPointer(uintptr_t* data, MonoListNode const* ptr);
// lirsCacheToggleStatus - switch HIR/LIR status of element
void lirsCacheToggleStatus(uintptr_t* data);
// lirsCacheIsLIR - returns if element == HIR 0, if == LIR 1
bool lirsCacheIsLIR(uintptr_t data);

// lirsCacheAddToHIR - adds new HIR element
CachePolicyAddResult lirsCacheAddToHIR(LIRSCache* LIRS, void const* key);
// lirsCacheAddToLIR - adds new LIR element 
CachePolicyAddResult lirsCacheAddToLIR(LIRSCache* LIRS, void const* key);
// lirsCacheAddToFull - adds new element to full_list
CachePolicyAddResult lirsCacheAddToFull(LIRSCache* LIRS, void const* key);
// lirsCacheDeleteFromFull - deletes key from full_list
void lirsCacheDeleteFromFull(LIRSCache* LIRS, void const* key);
// lirsCacheDeleteFromHIR - deletes key from hir_list
void lirsCacheDeleteFromHIR(LIRSCache* LIRS, void const* key);
// lirsCachePopHIR - deletes last hir element from hir_list
void lirsCachePopHIR(LIRSCache* LIRS, void* replace);
void lirsCachePopFull(LIRSCache* LIRS);

// lirsCacheMoveToFrontHIR - moves node_prev->next to front of hir_list
void lirsCacheMoveToFrontHIR(LIRSCache* LIRS, MonoListNode* node_prev);
// lirsCacheMoveToFrontFull - moves prev_ptr->next to front of full_list
void lirsCacheMoveToFrontFull(LIRSCache* LIRS, uintptr_t* prev_ptr);
CachePolicyAddResult lirsCacheAddToHirList(LIRSCache* LIRS, void const* key);
void lirsCacheUpdateNextNext(LIRSCache* LIRS, MonoListNode* prev);

struct LIRSCache_T {
    BaseOHT* full_table;
    BaseOHT* hir_table;
    MonoList* full_list;
    MonoList* hir_list;
    size_t capacity;
    size_t LIR_capacity;
    size_t HIR_capacity;
};

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
LIRSCache* lirsCacheAlloc(size_t capacity, size_t element_size, size_t element_align, BaseHashFunc hash_func, BaseCompareFunc compare_func)
{
    LIRSCache* LIRS = calloc(1, sizeof(*LIRS));
    if (!LIRS)
        return NULL;
    const double LIR_percent = 0.9;
    LIRS->capacity = capacity;
    LIRS->LIR_capacity = LIR_percent * LIRS->capacity; 
    LIRS->HIR_capacity = (1 - LIR_percent) * LIRS->capacity;

    LIRS->full_table = baseOHTInit(element_size, element_align, sizeof(uintptr_t), alignof(uintptr_t), hash_func, compare_func);
    if (!LIRS->full_table) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    LIRS->hir_table = baseOHTInit(element_size, element_align, sizeof(MonoListNode*), alignof(MonoListNode*), hash_func, compare_func);
    if (!LIRS->hir_table) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    LIRS->full_list = monoListAlloc(element_size);
    if (!LIRS->full_list) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    void* sent = calloc(1, element_size);
    if (!sent) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    MonoListNode* check = monoListAddToFront(LIRS->full_list, sent);
    if (!check) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    LIRS->hir_list = monoListAlloc(element_size);
    if (!LIRS->hir_list) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    check = monoListAddToFront(LIRS->hir_list, sent);
    if (!check) {
        lirsCacheFree(LIRS);
        return NULL;
    }
    return LIRS;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheFree(LIRSCache* LIRS)
{
    if (LIRS) {
        baseOHTFree(LIRS->full_table);
        baseOHTFree(LIRS->hir_table);
        monoListFree(LIRS->full_list);
        monoListFree(LIRS->hir_list);
        free(LIRS);
    }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lirsCacheContains(LIRSCache const* LIRS, void const* key)
{
    uintptr_t const* const temp = baseOHTFind(LIRS->full_table, key);
    
    if (temp == NULL)
        return false;
    uintptr_t data = *temp;

    if(lirsCacheIsLIR(data))
        return true;

    return baseOHTFind(LIRS->hir_table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheMoveToFrontFull(LIRSCache* LIRS, uintptr_t* prev_ptr)
{
    MonoListNode* prev = lirsCacheGetPointer(*prev_ptr);
    MonoListNode* cur = monoListNodeNext(prev);
    assert(cur);
    if (cur == monoListBack(LIRS->full_list))
        return;
    lirsCacheUpdateNextNext(LIRS, prev);

    MonoListNode* old_back = monoListBack(LIRS->full_list);
    lirsCacheSetPointer(prev_ptr, old_back);

    monoListMoveNextToBack(LIRS->full_list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheMoveToFrontHIR(LIRSCache* LIRS, MonoListNode* node_prev)
{
    MonoListNode* next = monoListNodeNext(node_prev);
    assert(next);
    MonoListNode** node_next_ptr = baseOHTFind(LIRS->hir_table, monoListConstNodeData(next));// check
    assert(node_next_ptr);
    memcpy(node_next_ptr, &node_prev, sizeof(uintptr_t));

    MonoListNode* old_front = monoListFront(LIRS->hir_list);
    MonoListNode** node_prev_ptr = baseOHTFind(LIRS->hir_table, monoListConstNodeData(next));
    memcpy(node_prev_ptr, &old_front, sizeof(uintptr_t)); // check

    monoListMoveNextToFront(LIRS->hir_list, node_prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheStackPrune(LIRSCache* LIRS)
{
    MonoListNode* end = monoListNodeNext(monoListFront(LIRS->full_list));
    while (true) {
        uintptr_t* end_data = baseOHTFind(LIRS->full_table, monoListConstNodeData(end));
        if (lirsCacheIsLIR(*end_data))
            break;
        lirsCacheDeleteFromFull(LIRS, monoListConstNodeData(end));
        end = monoListNodeNext(monoListFront(LIRS->full_list));
    }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddOrReplace(LIRSCache* LIRS, void const* key, void* replace)
{
    // When adding a key to the cache, the following can happen:
    // 1) This is a resident LIR block.
    //    Move it to the front of the queue and prune the stack.
    // 2) This is a non-resident block.
    //    If the number of LIR blocks is less than the maximum amount,
    //    add it as a LIR block.
    //    If the number of HIR blocks is less the the maximum amount,
    //    add it as a HIR block and carry on.
    //    Otherwise, evict a HIR block, add it as a HIR block and carry on.
    // 3) This is a resident HIR block.
    //    If this block is present in the queue, move it to the front of the queue,
    //    promote it to a LIR block, demote the last block of the queue to a HIR block,
    //    and prune the stack.
    //    If it is not present in the queue, and it to the queue.
    uintptr_t* full_value = baseOHTFind(LIRS->full_table, key);
    if (full_value && lirsCacheIsLIR(*full_value)) {
        lirsCacheMoveToFrontFull(LIRS, full_value);
        lirsCacheStackPrune(LIRS);
        return CACHE_POLICY_ADD_NO_REPLACE;
    }
    MonoListNode** hir_value = baseOHTFind(LIRS->hir_table, key);
    CachePolicyAddResult res = CACHE_POLICY_ADD_NO_REPLACE;
    if (!hir_value) {
        if (monoListSize(LIRS->full_list) < LIRS->LIR_capacity) {
            return lirsCacheAddToLIR(LIRS, key);
        }
        if (monoListSize(LIRS->hir_list) == LIRS->HIR_capacity) {
            lirsCachePopHIR(LIRS, replace);
            res = CACHE_POLICY_ADD_REPLACE;
        }
        if (lirsCacheAddToHIR(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
            return CACHE_POLICY_ADD_ERROR;
        }
        hir_value = baseOHTFind(LIRS->hir_table, key);
        assert(hir_value);
    }

    if (!full_value) {
        if (lirsCacheAddToFull(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
            return CACHE_POLICY_ADD_ERROR;
        }
        return res;
    }
    
    lirsCacheMoveToFrontFull(LIRS, full_value);
    lirsCacheToggleStatus(full_value);
    void const* oldest_lir_key = monoListConstNodeData(monoListNodeNext(monoListFront(LIRS->full_list)));
    if (lirsCacheAddToHirList(LIRS, oldest_lir_key) == CACHE_POLICY_ADD_ERROR) {
        return CACHE_POLICY_ADD_ERROR;
    }
    lirsCachePopFull(LIRS);
    lirsCacheStackPrune(LIRS);

    return res;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
// TODO: maybe replace should not be passed
void lirsCachePopHIR(LIRSCache* LIRS, void* replace)
{
    MonoListNode const* old_end = monoListConstBack(LIRS->hir_list);
    void const* old_data = monoListConstNodeData(old_end);
    memcpy(replace, old_data, monoListItemSize(LIRS->hir_list));
    lirsCacheDeleteFromHIR(LIRS, old_data);
}
void lirsCachePopFull(LIRSCache* LIRS) {
    MonoListNode const* pop_node = monoListNodeNext(monoListFront(LIRS->full_list));
    assert(pop_node);
    lirsCacheDeleteFromFull(LIRS, monoListConstNodeData(pop_node));
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheDeleteFromFull(LIRSCache* LIRS, void const* key) {
    uintptr_t* prev_data = baseOHTFind(LIRS->full_table, key);
    assert(prev_data);
    MonoListNode* prev = lirsCacheGetPointer(*prev_data);

    lirsCacheUpdateNextNext(LIRS, prev);
    
    monoListDeleteNext(LIRS->full_list, prev);
    
    baseOHTDelete(LIRS->full_table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheUpdateNextNext(LIRSCache* LIRS, MonoListNode* prev)
{
    MonoListNode* cur = monoListNodeNext(prev);
    assert(cur);
    MonoListNode* next = monoListNodeNext(cur);
    if (next) {
        uintptr_t* cur_data = baseOHTFind(LIRS->full_table, monoListConstNodeData(next));
        assert(cur_data);
        lirsCacheSetPointer(cur_data, prev);
    }
}
void lirsCacheDeleteFromHIR(LIRSCache* LIRS, void const* key) {
    MonoListNode** prev = baseOHTFind(LIRS->hir_table, key);
    assert(prev);
    MonoListNode* cur = monoListNodeNext(*prev);
    assert(cur);
    MonoListNode* next = monoListNodeNext(cur);
    if (next) {
        MonoListNode** next_ptr = baseOHTFind(LIRS->hir_table, monoListConstNodeData(next));
        assert(next_ptr);
        next_ptr = prev;
        // memcpy(next_ptr, prev, sizeof(uintptr_t));
    }
    monoListDeleteNext(LIRS->hir_list, *prev);
    
    baseOHTDelete(LIRS->hir_table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddToFull(LIRSCache* LIRS, void const* key) {
    uintptr_t old_front = (uintptr_t)monoListFront(LIRS->full_list);
    uintptr_t* value = baseOHTInsert(LIRS->full_table, key, &old_front);
    if (!value) {
        return CACHE_POLICY_ADD_ERROR;
    }

    MonoListNode* new_front = monoListAddToFront(LIRS->full_list, key);
    if (!new_front) {
        baseOHTDelete(LIRS->full_table, key);
        return CACHE_POLICY_ADD_ERROR;
    }

    return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddToHirList(LIRSCache* LIRS, void const* key) {
    MonoListNode* old_back = monoListBack(LIRS->hir_list);
    MonoListNode* value = baseOHTInsert(LIRS->hir_table, key, &old_back);
    if (!value) {
        return CACHE_POLICY_ADD_ERROR;
    }
    MonoListNode* new_back = monoListAddToBack(LIRS->hir_list, key);
    if (!new_back) {
        baseOHTDelete(LIRS->hir_table, key);
        return CACHE_POLICY_ADD_ERROR;
    }
    
    return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddToLIR(LIRSCache* LIRS, void const* key)
{  
    CachePolicyAddResult res = lirsCacheAddToFull(LIRS, key);
    if (res != CACHE_POLICY_ADD_ERROR) {
        uintptr_t* value = baseOHTFind(LIRS->full_table, key);
        assert(value);
        lirsCacheToggleStatus(value);
    }
    return res;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddToHIR(LIRSCache* LIRS, void const* key)
{
    CachePolicyAddResult res = lirsCacheAddToFull(LIRS, key);
    if (res != CACHE_POLICY_ADD_ERROR) {
        if (lirsCacheAddToHirList(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
            lirsCacheDeleteFromFull(LIRS, key);
            return CACHE_POLICY_ADD_ERROR;
        }
    }

    return res;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
MonoListNode* lirsCacheGetPointer(const uintptr_t data) {
    return (MonoListNode*) (data & (UINTPTR_MAX - 1));
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheToggleStatus(uintptr_t* data)
{
    *data = *data ^ 1;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lirsCacheIsLIR(uintptr_t data)
{
    return data & 1;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheSetPointer(uintptr_t* data, MonoListNode const* node)
{
    uintptr_t data_status = lirsCacheIsLIR(*data);
    *data = (uintptr_t) node;
    *data = *data ^ data_status;
}
