//BaseDoubleList realisation
#include "BaseDoubleList.h"

struct node_t {
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
	list->key_size = key_size;
	
	return list;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListAddFront(DoubleList* list, void const* value)
{
	assert(list && "DoubleList has not been defined!");
	doubleListPrepend(list);
	memcpy(list->start->data, value, list->key_size);
	
	return list->start;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListPrepend(DoubleList* list)
{
	assert(list && "DoubleList has not been defined!");
	DoubleListNode* node = calloc(1, sizeof(*node) + list->key_size);

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
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* doubleListPopBack(DoubleList* list)
{
	assert(list->end && "DoubleListNode is already empty!");
	if (list->end->prev == NULL) {
		free(list->end);
		list->length--;
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
	assert(list && "DoubleList has not been defined!");
	list->start->prev = doubleListRemove(list, node);
	node->next = list->start;
	node->prev = NULL;

	list->start = node;

	list->length++; // update cur_size, maybe no need
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
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list->length--; // update cur_size, maybe no need
	return node;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListFree(DoubleList* list)
{
	assert(list && "DoubleList is already empty!");
	DoubleListNode* tmp; 
	while (list->start != NULL) {
		tmp = list->start->next;
		free(list->start->data);
		free(list->start);
		list->start = tmp;
	}
		
	free(list);
}