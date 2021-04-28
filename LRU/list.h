// LRU list - liner
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_CASH_SZ 1000 //

#ifndef CURSZ_GUARD__
#define CURSZ_GUARD__
//int cur_size = 0;
#endif
struct node_t {
	struct node_t* next;
	struct node_t* prev;
	int* data;
};
// start == newest_one, start->prev = null
void make_list(struct node_t* start);
void free_list(struct node_t* start);
// insert_in_front - returns new position of start, src - source of data
struct node_t* insert_in_front(struct node_t* start, const void* src);
// move_forward - moves founnd element to the start, returns new position of start
struct node_t* move_forward(struct node_t* cur, struct node_t* start);

void show_list(struct node_t* start); // needs data-type function in arguments