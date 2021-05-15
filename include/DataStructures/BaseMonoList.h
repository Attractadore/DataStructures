#pragma once
#include <stddef.h>

typedef struct MonoList_T MonoList;
typedef struct MonoListNode_t MonoListNode;

MonoList* monoListAlloc(size_t key_size);
//allocates a list
void monoListFree(MonoList* ml);
//frees a list
MonoListNode* monoListAddToFront(MonoList* ml, void const* value);
//allocates a new node and puts it to the front as a start
MonoListNode* monoListAddToBack(MonoList* ml, void const* value);
//allocates a new node and puts it to the back as an end
MonoListNode* monoListPopBack(MonoList* ml);
//removes the node from the back and deletes it
MonoListNode* monoListPopFront(MonoList* ml);
//removes the node from the front and deletes it
MonoListNode* monoListMoveNextToFront(MonoList* ml, MonoListNode* prenode);
//removes the next node and puts it to the front as a start
MonoListNode* monoListMoveNextToBack(MonoList* ml, MonoListNode* prenode);
//removes the next node and puts it to the back as an end
MonoListNode* monoListRemoveNext(MonoList* ml, MonoListNode* prenode);
//removes the next node without deleting, returns pointer to the removed node
MonoListNode* monoListDeleteNext(MonoList* ml, MonoListNode* prenode);
//removes the next node and deletes it, returns new prenode->next
MonoListNode* monoListFront(MonoList* ml);
//returns list->start
MonoListNode* monoListBack(MonoList* ml);
//returns list->end
size_t monoListSize(MonoList* ml);
//returns list->length
size_t monoListItemSize(MonoList* ml);
//returns list->key_size
void* monoListNodeData(MonoListNode* node);
//returns node->data
MonoListNode const* monoListConstFront(MonoList const* ml);
//here and further - the same, but everything is const
MonoListNode const* monoListConstBack(MonoList const* ml);
void const* monoListConstNodeData(MonoListNode const* node);
MonoListNode* monoListPrepend(MonoList* ml, MonoListNode* node);
MonoListNode* monoListAppend(MonoList* ml, MonoListNode* node);
MonoListNode* monoListNodeNext(MonoListNode* node);
