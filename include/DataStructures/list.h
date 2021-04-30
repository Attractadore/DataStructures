#pragma once

#include <stddef.h>

typedef struct DoubleList_T DoubleList;

typedef struct node_t DoubleListNode;

DoubleList* doubleListAlloc(size_t key_size);
void doubleListFree(DoubleList* list); //+

DoubleListNode* doubleListAddFront(DoubleList* list, void const* value); //+
// void doubleListPrepend(DoubleList* list, DoubleListNode* node);

// DoubleListNode* doubleListRemove(DoubleList* list, DoubleListNode* node);
DoubleListNode* doubleListPopBack(DoubleList* list); //+

DoubleListNode* doubleListMoveToFront(DoubleList* list, DoubleListNode* node); //+