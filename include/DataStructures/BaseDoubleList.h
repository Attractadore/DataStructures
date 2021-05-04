#pragma once

#include <stddef.h>

typedef struct DoubleList_T DoubleList;
typedef struct DoubleListNode_T DoubleListNode;

//typedef enum type TypeOfValue, for tests

typedef enum type {
	TYPE_OF_VALUE_INT,
	TYPE_OF_VALUE_UINT,
	TYPE_OF_VALUE_LINT,
	TYPE_OF_VALUE_FLOAT,
	TYPE_OF_VALUE_STRING,
}TypeOfValue;


// doubleListAlloc - creating list with sizeof(element) = key_size, returns list pointer
DoubleList* doubleListAlloc(size_t key_size); //+
// doubleListFree - free list
void doubleListFree(DoubleList* list); //+

// doubleListAddFront - copy value in new node in front of list, returns new list->start
DoubleListNode* doubleListAddFront(DoubleList* list, void const* value); //+
// doubleListPrepend - adds node in front of list, returns new list->start
DoubleListNode* doubleListPrepend(DoubleList* list, DoubleListNode* node);

// doubleListRemove - removes node from list and returns its pointer
DoubleListNode* doubleListRemove(DoubleList* list, DoubleListNode* node);//+
// doubleListPopBack - free list->end, returns new list->end
DoubleListNode* doubleListPopBack(DoubleList* list); //+

// doubleListMoveToFront - bring node to the front of list, retuns list->start = node
DoubleListNode* doubleListMoveToFront(DoubleList* list, DoubleListNode* node); //+

// doubleListShowList - prints list, send as "type" const TYPE_OF_VALUE_"your type"
void doubleListShowList(DoubleList* list, TypeOfValue type);
