//BaseDoubleList realisation
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
// #include "C:\new_code\DataStructures\include\DataStructures\BaseDoubleList.h"
#include "BaseDoubleList.h"


/*
struct DoubleListNode_T {
	DoubleListNode* next;
	DoubleListNode* prev;
	unsigned char data[];
};

 struct DoubleList_T {
	DoubleListNode* start;
	DoubleListNode* end;
	size_t key_size;
	size_t length;
 };
 */
 /*------------------------------------------------------------------------------------------------------------------------------*/
DoubleList* doubleListAlloc(size_t key_size)
{
	DoubleList* list = calloc(1, sizeof(DoubleList));
	if (!list)
		return NULL;
	list->key_size = key_size;

	return list;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListAddFront(DoubleList* list, void const* value)
{
	DoubleListNode* node = calloc(1, sizeof(DoubleListNode) + list->key_size);
	if(!node)
		return NULL;
	memcpy(node->data, value, list->key_size);
	
	return doubleListPrepend(list, node);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListPrepend(DoubleList* list, DoubleListNode* node)
{
	assert(node);
	if (list->start == NULL) {
		list->start = node;
		list->end = list->start;
	}
	else {
		list->start->prev = node;
		node->next = list->start;
		list->start = node;
	}
	list->length++; // update cur_size
	return list->start;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListPopBack(DoubleList* list)
{
	assert(list->end && "DoubleList is already empty!");
	if (list->end == list->start) {
		free(list->end);
		list->end = NULL;
		list->start = NULL;
		list->length--;
		return NULL;
	}
	list->end = list->end->prev;
	free(list->end->next);
	list->end->next = NULL;
	list->length--;
	return list->end;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListMoveToFront(DoubleList* list, DoubleListNode* node) 
{
	doubleListPrepend(list, doubleListRemove(list, node));
	
	return list->start;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListShowList(DoubleList* list, TypeOfValue type)
{
	if (list->start == NULL)
		printf("List is empty!\n");
	else {
		DoubleListNode* tmp = list->start;
		while (tmp != NULL) {
			switch (type) {
			case TYPE_OF_VALUE_INT: {
				printf("%d\n", *(int*)(tmp->data));
				break;
			}
			case TYPE_OF_VALUE_UINT: {
				printf("%u\n", *(unsigned int*)(tmp->data));
				break;
			}
			case TYPE_OF_VALUE_LINT: {
				printf("%lu\n", *(unsigned long*)(tmp->data));
				break;
			}
			case TYPE_OF_VALUE_FLOAT: {
				printf("%g\n", *(float*)(tmp->data));
				break;
			}
			case TYPE_OF_VALUE_STRING: {
				printf("%s\n", (char*)(tmp->data));
				break;
			}	
			default:
				printf("Error type\n");
				break;
			}
			tmp = tmp->next;
		}

		printf("\n");
	}
}

/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListRemove(DoubleList* list, DoubleListNode* node)
{
	assert(node && "Error: node == nullptr");
	if (node->prev == NULL) {
		list->start = node->next;
		node->next->prev = NULL;
		return node;
	}
	if (node->next == NULL) {
		list->end = node->prev;
		node->prev->next = NULL;
		return node;
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->next = NULL;
	node->prev = NULL;
	return node;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListFree(DoubleList* list) // Debug
{
	if (list) {
		DoubleListNode* tmp = list->start, *next;
		while (tmp) {
			next = tmp->next;
			free(tmp);
			tmp = next;
		}
		free(list);
	}
}