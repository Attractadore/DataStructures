#include "LIRSCache.h"
#include "BaseMonoList.h"
#include "BaseOpenHashTable.h"

#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

#define HIR 0
#define LIR 1  

// lirsCacheGetPointer - cast data to list_pointer
MonoListNode* lirsCacheGetPointer(uintptr_t data);
// lirsCacheToggleStatus - switch HIR/LIR status of element
void lirsCacheToggleStatus(uintptr_t data);
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


struct LIRSCache_T {
    BaseOHT* full_table;
    BaseOHT* hir_table;
    MonoList* full_list;
    MonoList* hir_list;
    size_t capacity;
    size_t LIR_capacity;
    size_t HIR_capacity;
    // solve sentiel problem
};

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
void lirsCacheMoveToFront(LIRSCache* LIRS, uintptr_t* prev_ptr)
{
    MonoListNode* prev = lirsCacheGetPointer(*prev_ptr);
    MonoListNode* next = monoListNextNode(prev);
    assert(next);
    uintptr_t* node_next_ptr = baseOHTFind(LIRS->full_table, monoListConstNodeData(next));
    assert(node_next_ptr);
    memcpy(node_next_ptr, prev_ptr, sizeof(uintptr_t));

    uintptr_t old_front = (uintptr_t)monoListFront(LIRS->full_list);
    memcpy(node_prev_ptr, &old_front, sizeof(uintptr_t)); // check

    monoListMoveNextToFront(LIRS->full_list, prev);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheStackPrune(LIRSCache* LIRS)
{
    uintptr_t end = (uintptr_t)monoListBack(LIRS->full_list);
    MonoList* end_ptr = lirsCacheGetPointer(end);
    while (!lirsCacheIsLIR(end)) {
        lirsCacheDeleteFromFull(LIRS, monoListConstNodeData(end_ptr));
        end = (uintptr_t)monoListBack(LIRS->full_list);
        end_ptr = lirsCacheGetPointer(end);
    }
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddOrReplace(LIRSCache* LIRS, void const* key, void* replace)
{
    uintptr_t const* const node_prev_ptr = baseOHTFind(LIRS->full_table, key);
    if (node_prev_ptr) {
        if (lirsCacheIsLIR(*node_prev_ptr)) { // LIR

            lirsCacheMoveToFront(LIRS, node_prev_ptr);
#if 0
            MonoListNode* prev = lirsCacheGetPointer(*node_prev_ptr);
            MonoListNode* next = monoListNextNode(prev);
            assert(next);
            uintptr_t* node_next_ptr = baseOHTFind(LIRS->full_table, monoListConstNodeData(next));
            assert(node_next_ptr);
            memcpy(node_next_ptr, node_prev_ptr, sizeof(uintptr_t));

            uintptr_t old_front = (uintptr_t)monoListFront(LIRS->full_list);
            memcpy(node_prev_ptr, &old_front, sizeof(uintptr_t)); // check

            monoListMoveNextToFront(LIRS->full_list, prev); 
#endif
            // what if that was the last LIR...
            uintptr_t end = (uintptr_t)monoListBack(LIRS->full_list);
            if(!lirsCacheIsLIR(end))
                lirsCacheStackPrune(LIRS);
            return CACHE_POLICY_NO_REPLACE;
        }
        else { // HIR
            lirsCacheToggleStatus(node_prev_ptr);
            MonoListNode* prev = lirsCacheGetPointer(*node_prev_ptr);
            MonoListNode* next = monoListNextNode(prev);
            lirsCacheDeleteFromHIR(LIRS, monoListConstNodeData(next));
            lirsCacheMoveToFront(LIRS, node_prev_ptr);
            const void* old_last_LIR_data = monoListConstNodeData(monoListBack(LIRS->full_list));
            uintptr_t* old_last_LIR = baseOHTFind(LIRS->full_table, old_last_LIR_data);
            lirsCacheToggleStatus(old_last_LIR);
            lirsCacheAddToHIR(LIRS, old_last_LIR_data);
            // ...
            lirsCacheStackPrune(LIRS);

            return CACHE_POLICY_ADD_NO_REPLACE;

        }

        node_prev_ptr = baseOHTFind(LIRS->hir_table, key); // if non-resident
        // шо дальше делать то
        if (node_prev_ptr) {
            // moveTofront of hir_list
            lirsCacheMoveToFrontHIR(LIRS, node_prev_ptr);

        }

    }   
#if 0
    uintptr_t const* const node_ptr = baseOHTFind(LIRS->full_table, key);
    if (node_ptr)
        node_ptr = baseOHTFind(LIRS->hir_table, key);
    if (node_ptr) {
        bool status = lirsCacheStatus(LIRS, *node_ptr);
        if(status) { // LIR
            monoListMoveNextToFront(LIRS->full_list, *node_ptr); // ...next
        }
        else { // HIR
            lirsCacheChangeStatus(LIRS, *node_ptr);
            MonoListNode* new_end = monoListPopBack(LIRS->full_list);
            // void* new_end_data = monoListNodeData(new_end);
            while (lirsCacheStatus(LIRS, new_end) == HIR) {
                baseOHTDelete(LIRS->full_table, monoListConstNodeData(new_end));
                if (baseOHTFind(LIRS->hir_table)) {
                    baseOHTDelete(LIRS->hir_table, monoListConstNodeData(new_end));
                    monoListPopBack(LIRS->hir_list);
                }
                new_end = monoListPopBack(LIRS->full_list); // need to delete all hir from end until lir met
            }
        }
        return CACHE_POLICY_ADD_NO_REPLACE;
    }
#endif

    if (LIRS->LIR_capacity > monoListSize(LIRS->full_list)) {
        return lirsCacheAddToLIR(LIRS, key);
    }
    if (LIRS->HIR_capacity > monoListSize(LIRS->hir_list) - LIR_capacity) { // check
        return lirsCacheAddToHIR(LIRS, key);
    }

    lirsCachePopHIR(LIRS, replace);
    
    if (lirsCacheAddToHIR(LIRS, key) == CACHE_POLICY_ADD_ERROR) {
        return CACHE_POLICY_ADD_ERROR;
    }
    return CACHE_POLICY_ADD_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCachePopHIR(LIRSCache* LIRS, void* replace)
{
    MonoListNode const* old_end = monoListConstBack(LIRS->hir_list);
    void const* old_data = monoListConstNodeData(old_end);
    memcpy(replace, old_data, monoListItemSize(LIRS->hir_list));
    lirsCacheDeleteFromHIR(LIRS, old_data);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheDeleteFromFull(LIRSCache* LIRS, void const* key) {
    uintptr_t* data = baseOHTFind(LIRS->full_table, key);
    assert(data);
    MonoListNode* prev = lirsCacheGetPointer(data);
    MonoListNode* next = monoListNextNode(prev);
    if (next) {
        if (monoListNextNode(next)) {
            uintptr_t* next_data = baseOHTFind(LIRS->full_table, monoListConstNodeData(next));
            assert(next_data);
            memcpy(next_data, data, sizeof(uintptr_t));
        }
        monoListDeleteNext(LIRS->full_list, prev);
    }
    baseOHTDelete(LIRS->full_table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lirsCacheDeleteFromHIR(LIRSCache* LIRS, void const* key) {
    MonoListNode** prev = baseOHTFind(LIRS->hir_table, key);
    assert(prev);
    MonoListNode* next = monoListNextNode(*prev);
    if (next) {
        if (monoListNextNode(next)) {
            MonoListNode** next_next = baseOHTFind(LIRS->hir_table, monoListConstNodeData(next));
            assert(next_next);
            memcpy(next_next, next, sizeof(uintptr_t));
        }
        monoListDeleteNext(LIRS->hir_list, *prev);
    }
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
        lirsCacheDeleteFromFull(LIRS, key);
        return CACHE_POLICY_ADD_ERROR;
    }
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
        MonoListNode* old_front = monoListFront(LIRS->hir_list, key);
        MonoListNode* value = baseOHTInsert(LIRS->hir_table, key, &old_front);
        if (!value) {
            lirsCacheDeleteFromFull(LIRS, key);
            return CACHE_POLICY_ADD_ERROR;
        }
        MonoListNode* new_front = monoListAddToFront(LIRS->hir_list, key);
        if (!new_front) {
            lirsCacheDeleteFromHIR(LIRS, key);
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