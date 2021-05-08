#pragma once
#include <stddef.h>

typedef struct MonoList_T MonoList;
typedef struct MonoListNode_t MonoListNode;

MonoList* monoListAlloc(size_t key_size);
void monoListFree(MonoList* ml);
MonoListNode* monoListAddToFront(MonoList* ml, void const* value);
MonoListNode* monoListAddToBack(MonoList* ml, void const* value);
MonoListNode* monoListPopBack(MonoList* ml);
MonoListNode* monoListPopFront(MonoList* ml);
MonoListNode* monoListMoveNextToFront(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListMoveNextToBack(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListRemoveNext(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListDeleteNext(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListFront(MonoList* ml);
MonoListNode* monoListBack(MonoList* ml);
size_t* monoListSize(MonoList* ml);
size_t* monoListItemSize(MonoList* ml);
void* monoListNodeData(MonoListNode* node);
MonoListNode const* monoListConstFront(MonoList const* ml);
MonoListNode const* monoListConstBack(MonoList const* ml);
void const* monoListConstNodeData(MonoListNode const* node);
