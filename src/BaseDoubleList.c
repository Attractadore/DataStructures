//BaseDoubleList realisation
#include "BaseDoubleList.h"

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
 /*------------------------------------------------------------------------------------------------------------------------------*/
DoubleList* doubleListAlloc(size_t key_size)
{
	DoubleList* list = calloc(1, sizeof(DoubleList) + key_size);
	assert(list);
	// list->key_size = key_size;
	
	return list;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListAddFront(DoubleList* list, void const* value)
{
	DoubleListNode* node = calloc(1, sizeof(DoubleListNode) + list->key_size);
	assert(node);
	memcpy(node->data, value, list->key_size);
	
	return doubleListPrepend(list, node);;
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
	if (list->end->prev == NULL) {
		free(list->end);
		list->length--;
		list->start = NULL;
		return NULL;
	}
	list->end = list->end->prev;
	free(list->end->next);
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
/*
void show_list(struct node_t* start)// for int only, not tested yet
{
	printf("%d ", *(start->data));
	if(start->next != NULL) {
		show_list(start->next);
	}
	else
		printf("\n");
}
*/
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListRemove(DoubleList* list, DoubleListNode* node)
{
	assert(node && "Error: node == nullptr");
	if (node->prev == NULL || node->next == NULL)
		return node;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	return node;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListFree(DoubleList* list)
{
	if (list) {
		DoubleListNode* tmp;
		while (list->start != NULL) {
			tmp = list->start->next;
			free(list->start);
			list->start = tmp;
		}

		free(list);
	}
}