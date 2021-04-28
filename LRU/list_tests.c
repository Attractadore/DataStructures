// tests for list.c
#include "list.h"

int main() {
	struct node_t* start = calloc(1, sizeof(struct node_t));
	int arr[10];

	for(int i = 0; i < 10; i++) {
		arr[i] = i;
		start = insert_in_front(start, (const void*)(&arr[i]));
	}
	show_list(start);
	struct node_t* cur = start->next;
	struct node_t* cur1 = cur->next;
	for(int i = 1; i < 6; i++) {

		start = move_forward(cur, start);
		cur = cur1;
		cur1 = cur->next;
	}
	show_list(start);
	free_list(start);

	return 0;
}