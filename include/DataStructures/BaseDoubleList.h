#pragma once

#include <stddef.h>
<<<<<<< HEAD
=======

>>>>>>> parent of c45bec9... commit
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct DoubleList_T DoubleList;
<<<<<<< HEAD
typedef struct DoubleListNode_T DoubleListNode;
=======
typedef struct node_t DoubleListNode;
>>>>>>> parent of c45bec9... commit

// doubleListAlloc - creating list with sizeof(element) = key_size, returns list pointer
DoubleList* doubleListAlloc(size_t key_size); //+
// doubleListFree - free list
void doubleListFree(DoubleList* list); //+

// doubleListAddFront - copy value in new node in front of list, returns new list->start
DoubleListNode* doubleListAddFront(DoubleList* list, void const* value); //+
<<<<<<< HEAD
// doubleListPrepend - adds node in front of list, returns new list->start
DoubleListNode* doubleListPrepend(DoubleList* list, DoubleListNode* node);
=======
// doubleListPrepend - creating new node in front of list
void doubleListPrepend(DoubleList* list);
>>>>>>> parent of c45bec9... commit

// doubleListRemove - removes node from list and returns its pointer
DoubleListNode* doubleListRemove(DoubleList* list, DoubleListNode* node);//+
// doubleListPopBack - free list->end, returns new list->end
DoubleListNode* doubleListPopBack(DoubleList* list); //+

// doubleListMoveToFront - bring node to the front of list, retuns list->start = node
<<<<<<< HEAD
DoubleListNode* doubleListMoveToFront(DoubleList* list, DoubleListNode* node); //+
=======
DoubleListNode* doubleListMoveToFront(DoubleList* list, DoubleListNode* node); //+
>>>>>>> parent of c45bec9... commit
