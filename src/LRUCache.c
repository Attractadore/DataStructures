#include "LRUCache.h"

struct LRUCache_T {
	BaseOHT* table;
	BaseVector* vector;
	DoubleList* list;
};
/*------------------------------------------------------------------------------------------------------------------------------*/
LRUCache* LRUCacheAlloc(size_t element_size, size_t key_align, size_t value_size, size_t value_align)
{
	LRUCache* LRU = calloc(1, sizeof(*LRU));
	LRU->table = baseOHTInit(element_size, key_align, sizeof(DoubleList), value_align, LRU_hash_func, LRU_hash_cmp);
	LRU->vector = baseVectorAlloc(elem_size);
	LRU->list = doubleListAlloc(element_size);
	return LRU;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
uint64_t LRU_hash_func(void const* key)
{
	uint64_t hash;
	//...//CRC-32
	return hash;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool LRU_hash_cmp(void const* left_key, void const* right_key)
{
	return left_key == right_key;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void LRU(void)
{
	LRUCache* LRU = LRUCacheAlloc(...); // for ints
	while (baseVectorGetSize((BaseVector const*)LRU->vector) > 0) {
		LRUstep(LRU);
	}
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void LRUstep(LRUCache* LRU)
{
	void const* new_element = LRUgetNextData(LRU);
	DoubleListNode* position = LRUsearchDoubleList(LRU, new_element);
	if (position) {
		// extern data from position and show it
		doubleListMoveToFront(LRU->list, position);
	}
	else {
		while (0) {// check if there is enough space in memory, free from end is no
			doubleListPopBack(LRU->list);
		}
		doubleListAddFront(LRU->list, (const void*)new_element); // need fix const void void const...
	// show data from new_element
	}
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* LRUsearchDoubleList(LRUCache* LRU, void const* key)// get hash
{
	return (DoubleListNode*)baseOHTFind(LRU->table, key); // baseOHTFindOrInsert
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void const* LRUgetNextData(LRUCache* LRU)
{
	return baseVectorConstData((BaseVector const*)LRU->vector);
}