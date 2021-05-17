/*
 * This describes a singly linked list
 */

#pragma once

#include <stddef.h>

typedef struct MonoList_T MonoList;
typedef struct MonoListNode_t MonoListNode;
// allocates a list
MonoList *monoListAlloc(size_t key_size);
// frees a list
void monoListFree(MonoList *ml);
// allocates a new node and puts it to the front as a start, returns list->start
MonoListNode *monoListAddToFront(MonoList *ml, void const *value);
// allocates a new node and puts it to the back as an end, returns list->end
MonoListNode *monoListAddToBack(MonoList *ml, void const *value);
// removes the node from the back and deletes it, returns list->end
MonoListNode *monoListPopBack(MonoList *ml);
// removes the node from the front and deletes it, returns list->start
MonoListNode *monoListPopFront(MonoList *ml);
// removes the next node and puts it to the front as a start, returns list->start
MonoListNode *monoListMoveNextToFront(MonoList *ml, MonoListNode *prenode);
// removes the next node and puts it to the back as an end, returns list->end
MonoListNode *monoListMoveNextToBack(MonoList *ml, MonoListNode *prenode);
// removes the next node without deleting, returns pointer to the removed node
MonoListNode *monoListRemoveNext(MonoList *ml, MonoListNode *prenode);
// removes the next node and deletes it, returns new prenode->next
MonoListNode *monoListDeleteNext(MonoList *ml, MonoListNode *prenode);
// returns list->start
MonoListNode *monoListFront(MonoList *ml);
// returns list->end
MonoListNode *monoListBack(MonoList *ml);
// returns list->length
size_t monoListSize(MonoList *ml);
// returns list->key_size
size_t monoListItemSize(MonoList *ml);
// returns node->data
void *monoListNodeData(MonoListNode *node);
// returns const list->start
MonoListNode const *monoListConstFront(MonoList const *ml);
// returns const list->end
MonoListNode const *monoListConstBack(MonoList const *ml);
// returns const node->data
void const *monoListConstNodeData(MonoListNode const *node);
// makes node be a new front, returns list->start
MonoListNode *monoListPrepend(MonoList *ml, MonoListNode *node);
// makes node be a new back, returns list->end
MonoListNode *monoListAppend(MonoList *ml, MonoListNode *node);
// returns node->next
MonoListNode *monoListNodeNext(MonoListNode *node);
