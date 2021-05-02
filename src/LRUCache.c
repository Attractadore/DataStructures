#include "LRUCache.h"

#include "BaseDoubleList.h"
#include "BaseOpenHashTable.h"
#include "BaseVector.h"

struct LRUCache_T {
	BaseOHT* table;
	BaseVector* vector;
	DoubleList* list;
};
/*------------------------------------------------------------------------------------------------------------------------------*/
LRUCache* LRUCacheAlloc(size_t element_size, size_t element_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func)
{
	LRUCache* LRU = calloc(1, sizeof(*LRU));
	LRU->table = baseOHTInit(element_size, element_align, value_size, value_align, hash_func, compare_func);
	LRU->vector = baseVectorAlloc(element_size);
	LRU->list = doubleListAlloc(element_size);
	return LRU;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
/*uint64_t LRU_hash_func(void const* key) // for ints, silly
{
	uint64_t hash = *((uint64_t*)key); // in demo.c lies normal hash_func...
	return hash;
}*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*bool LRU_hash_cmp(void const* left_key, void const* right_key)
{
	return left_key == right_key;
}*/
/*------------------------------------------------------------------------------------------------------------------------------*/
void LRUCache(size_t element_size, size_t element_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func)
{
	LRUCache* LRU = LRUCacheAlloc(element_size, element_align, value_size, value_align, hash_func, compare_func));
	while (baseVectorGetSize((BaseVector const*)LRU->vector) > 0) {
		LRUstep(LRU);
	}
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void LRUCacheStep(LRUCache* LRU)
{
	void const* new_element = LRUgetNextData(LRU);
	DoubleListNode* position = LRUsearchDoubleList(LRU, new_element);
	if (position) {
		// extern data from position and show it
		doubleListMoveToFront(LRU->list, position);
	}
	else {
		while (0) {// check if there is enough space in memory, free from end if no
			doubleListPopBack(LRU->list);
		}
		doubleListAddFront(LRU->list, (const void*)new_element); // need fix const void void const...
	// show data from new_element
	}
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode* LRUsearchInDoubleList(LRUCache* LRU, void const* key)// get hash
{
	return (DoubleListNode*)baseOHTFindOrInsert(LRU->table, key); // baseOHTFindOrInsert
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void const* LRUgetNextData(LRUCache* LRU)
{
	return baseVectorConstData((BaseVector const*)LRU->vector);
}