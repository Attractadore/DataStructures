//list LRU realisation
#include "list.h"

// remove_from_end - removes last used element from the cash
void remove_from_end(struct node_t* start);
int cur_size = 0;

struct node_t* insert_in_front(struct node_t* start, const void* src) // need optimization
{
	if(start->data == NULL) {
		start = calloc(1, sizeof(struct node_t));
		start->data = calloc(1, sizeof((char*)src));
	}
	else {
		start->prev = calloc(1, sizeof(struct node_t));
		while(cur_size + sizeof((char*)src) > MAX_CASH_SZ)
			remove_from_end(start);
		start->prev->data = calloc(1, sizeof((char*)src));
		start->prev->next = start;
		start = start->prev;
	}
	memcpy(start->data, src, sizeof((char*)src));///!!!!!!!!!
	cur_size += sizeof((char*)src); // update cur_size
	return start;
}

void remove_from_end(struct node_t* start)//done
{
	assert(start);
	while(start->next != NULL)
		start = start->next;
	cur_size -= sizeof(start->data); // update cur_size
	free(start->data);
	free(start);
}

struct node_t* move_forward(struct node_t* cur, struct node_t* start)//done
{
	assert(start);
	assert(cur);

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	assert(start->prev == NULL);
	start->prev = cur;
	cur->next = start;
	cur->prev = NULL;
	return cur;
}

void show_list(struct node_t* start)// for int only, not tested yet
{
	printf("%d ", *(start->data));
	if(start->next != NULL) {
		show_list(start->next);
	}
	else
		printf("\n");
}

void free_list(struct node_t* start)//done
{
	assert(start);
	if(start->next != NULL)
		free_list(start->next);
	free(start->data);
	free(start);
}