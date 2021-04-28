//list LRU realisation
#include "clist.h"

// remove_from_end - removes last used element from the cash
void remove_from_end(struct cycle_t* start);

struct cycle_t* insert_in_front(struct cycle_t* start, const void* src) // need optimization
{
	assert(start);
	while(cur_size + sizeof((char*)src) > MAX_CASH_SZ) {
		remove_from_end(start);
	}
	start->prev->next = calloc(1, sizeof(struct cycle_t));
	start->prev->next->data = calloc(1, sizeof((char*)src));
	start->prev->next->next = start;
	start->prev->next->prev = start->prev;
	start->prev = start->prev->next;

	memcpy(start->prev->next->data, src, sizeof((char*)src));///!!!!!!!!!
	cur_size += sizeof((char*)src); // update cur_size
	return start->prev;
}

void remove_from_end(struct cycle_t* start)//done
{
	assert(start);
	start = start->prev;
	cur_size -= sizeof(start->data); // update cur_size
	start->prev->next = start->next;
	start->next->prev = start->prev;

	free(start->data);
	free(start);
}

struct cycle_t* move_forward(struct cycle_t* cur, struct cycle_t* start)//done
{
	assert(start);
	assert(cur);

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	
	start->prev->next = cur;
	cur->prev = start->prev;
	start->prev = cur;
	cur->next = start;

	return cur;
}

void show_list(struct cycle_t* start)// for int only, not tested yet
{
	printf("%d ", *(start->data));
	if(start->next != NULL) {
		show_list(start->next);
	}
	else
		printf("\n");
}

void free_list(struct cycle_t* cur, struct cycle_t* start)//done
{
	assert(cur);

	if(cur->next != cur)
		free_list(cur->next, start);
	free(cur->data);
	free(cur);
}