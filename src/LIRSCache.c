#include "LIRSCache.h"
#include "BaseMonoList.h"
#include "BaseOpenHashTable.h"

#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

#define HIR 0
#define LIR 1  

MonoListNode* lirsCacheGetPointer(uintptr_t data);
// lirsCacheChangeStatus - switch HIR/LIR status of element
void lirsCacheToggleStatus(uintptr_t data);
// lirsCacheStatus - returns if element == HIR 0, if == LIR 1, сделать бы #define LIR 1
bool lirsCacheIsLIR(uintptr_t data);

bool lirsCacheStatus2(MonoListNode* node);
void lirsCacheChangeStatus2(MonoListNode* node);

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
void lirsCachePopHIR(LIRSCache* LIRS, void* replace)
{
    MonoListNode const* old_end = monoListConstBack(LIRS->hir_list);
    void const* old_data = monoListConstNodeData(old_end);
    memcpy(replace, old_data, monoListItemSize(LIRS->hir_list));
    baseOHTDelete(LIRS->hir_table, old_data);
    monoListDeleteNext(LIRS->hir_list, monoListFront(LIRS->hir_list)); // sentiel
    // monoListPopBack(LIRS->hir_list);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddOrReplace(LIRSCache* LIRS, void const* key, void* replace)
{
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
        lirsCacheAddLIR();
    }
    if (LIRS->HIR_capacity > monoListSize(LIRS->hir_list)) {
        lirsCacheAddHIR();
    }

    lirsCachePopHIR(LIRS, replace);
    
    if (lirsCacheAddHIR() == CACHE_POLICY_ADD_ERROR) {
        return CACHE_POLICY_ADD_ERROR;
    }
    return CACHE_POLICY_ADD_REPLACE;



    /*
        }
        else
            lirsCacheAdd(LIRS, key, HIR);
    }

    return lirsCacheAdd(LIRS, key, LIR);
    */
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAddToFull(LIRSCache* LIRS, void const* key) {
    uintptr_t old_end = (uintptr_t)monoListBack(LIRS->full_list);
    uintptr_t* value = baseOHTInsert(LIRS->full_table, key, &old_end);
    if (!value) {
        return CACHE_POLICY_ADD_ERROR;
    }

    MonoListNode* new_back = monoListAddToBack(LIRS->full_list, key);
    if (!new_back) {
        baseOHTDelete(LIRS->full_table, key);
        return CACHE_POLICY_ADD_ERROR;
    }
}

void lirsCacheDeleteFromFull(LIRSCache* LIRS, void const* key) {
    uintptr_t* data = baseOHTFind(LIRS->full_table, key);
    assert(data);
    // TODO: add function for retrieving the pointer
    MonoListNode* prev = lirsCacheGetPointer(data);
    monoListDeleteNext(LIRS->full_list, prev);
    baseOHTDelete(LIRS->full_table, key);
}
CachePolicyAddResult lirsCacheAddLIR(LIRSCache* LIRS, void const* key)
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
CachePolicyAddResult lirsCacheAddHIR(LIRSCache* LIRS, void const* key)
{
    CachePolicyAddResult res = lirsCacheAddToFull(LIRS, key);
    if (res != CACHE_POLICY_ADD_ERROR) {
        MonoListNode* old_end = monoListBack(LIRS->hir_list, key);
        MonoListNode* value = baseOHTInsert(LIRS->hir_table, key, &old_end);
        if (!value) {
            lirsCacheDeleteFromFull(LIRS, key);
            return CACHE_POLICY_ADD_ERROR;
        }
        MonoListNode* new_back = monoListAddToBack(LIRS->full_list, key);
        if (!new_back) {
            baseOHTDelete(LIRS->hir_table, key);
            lirsCacheDeleteFromFull(LIRS, key);
            return CACHE_POLICY_ADD_ERROR;
        }
    }
    return res;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lirsCacheAdd(LIRSCache* LIRS, void const* key, int status) // lirsCacheAddLIR and lirsCacheAddHIR
{
    if (status == HIR) {
        if (!baseOHTInsert(LIRS->hir_table, key, &front)) { // pass old_front element
            monoListPopFront(LIRS->hir_list);
            return CACHE_POLICY_ADD_ERROR;
        }
        monoListAddToFront(LIRS->hir_list, &front); // keep key in hir list
    }
    else {
        void* new_data = monoListNodeData(front);
        lirsCacheChangeStatus(LIRS, new_data);
    }

    return CACHE_POLICY_ADD_NO_REPLACE;
}
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