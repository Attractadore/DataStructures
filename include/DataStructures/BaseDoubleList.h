#pragma once

#include <stddef.h>

typedef struct DoubleList_T DoubleList;
typedef struct DoubleListNode_T DoubleListNode;

// doubleListAlloc - creating list with sizeof(element) = key_size, returns list
// pointer
DoubleList *doubleListAlloc(size_t key_size); //+
// doubleListFree - free list
void doubleListFree(DoubleList *list); //+

// doubleListAddFront - copy value in new node in front of list, returns new
// list->start
DoubleListNode *doubleListAddFront(DoubleList *list, void const *value); //+
// doubleListPrepend - adds node in front of list, returns new list->start
DoubleListNode *doubleListPrepend(DoubleList *list, DoubleListNode *node);

// doubleListRemove - removes node from list and returns its pointer
DoubleListNode *doubleListRemove(DoubleList *list, DoubleListNode *node); //+
// doubleListDelete - remove and free node from list
void doubleListDelete(DoubleList *list, DoubleListNode *node);
// doubleListPopFront - free list->front and return the new front
DoubleListNode *doubleListPopFront(DoubleList *list);
// doubleListPopBack - free list->end, returns new list->end
DoubleListNode *doubleListPopBack(DoubleList *list); //+

// doubleListMoveToFront - bring node to the front of list, retuns list->start =
// node
DoubleListNode *doubleListMoveToFront(DoubleList *list,
                                      DoubleListNode *node); //+

// doubleListSize - returns current length of list
size_t doubleListSize(DoubleList const *list);
// doubleListNodeData - returns data from node
void const *doubleListNodeConstData(DoubleListNode const *node);
// doubleListBack - returns node from end of the list
DoubleListNode const *doubleListConstBack(DoubleList const *list);
// doubleListItemSize - returns size of elements from list
size_t doubleListItemSize(DoubleList const *list);
