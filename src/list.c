//list realisation
#include "list.h"

typedef struct node_t {
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
DoubleList* doubleListAlloc(size_t key_size)
{
	DoubleList* list = calloc(1, sizeof(DoubleList) + key_size);
	list->key_size = key_size;
	
	return list;
}
DoubleListNode* doubleListAddFront(DoubleList* list, void const* value)
{
	DoubleListNode* cur = calloc(1, sizeof(*cur) + list->key_size);
	memcpy(list->start->data, value, list->key_size);
	if (list->start == NULL) {
		list->start = cur;
		list->end = list->start;
	}
	else {
		list->start->prev = cur;
		cur->next = list->start;
		list->start = cur;
	}

	
	list->length++; // update cur_size
	return list->start;
}
DoubleListNode* doubleListPopBack(DoubleList* list)
{
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

DoubleListNode* doubleListMoveToFront(DoubleList* list, DoubleListNode* node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list->start->prev = node;
	node->next = list->start;
	node->prev = NULL;

	list->start = node;
	return list->start;
}

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

void doubleListFree(DoubleList* list)
{
	DoubleListNode* tmp; 
	while (list->start != NULL) {
		tmp = list->start->next;
		free(list->start->data);
		free(list->start)
		list->start = tmp;
	}
		
	free(list)
}