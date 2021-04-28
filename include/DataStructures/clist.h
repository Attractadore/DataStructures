// LRU cycle list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_CASH_SZ 1000 //

int cur_size = 0;

struct cycle_t {
	struct cycle_t* next;
	struct cycle_t* prev;
	int* data;
};
// start == newest_one, start->prev = last one
void make_list(struct cycle_t* start);
void free_list(struct cycle_t* cur, struct cycle_t* start);
// insert_in_front - returns new position of start, src - source of data
struct cycle_t* insert_in_front(struct cycle_t* start, const void* src);
// move_forward - moves found element to the start, returns new position of start
struct cycle_t* move_forward(struct cycle_t* cur, struct cycle_t* start);

void show_list(struct cycle_t* start); // needs data-type function in arguments