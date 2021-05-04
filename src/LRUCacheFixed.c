#include "BaseDoubleList.h"
#include "BaseOpenHashTable.h"
#include "LRUCacheFixed.h"

struct LRUCache_T {
	BaseOHT* table;
	DoubleList* list;
	size_t capacity;
	void const* old_key;
};
/*------------------------------------------------------------------------------------------------------------------------------*/
LRUCache* lruCacheAlloc(size_t capacity, size_t element_size, size_t element_align, BaseHashFunc hash_func, BaseCompareFunc compare_func)
{
	LRUCache* LRU = calloc(1, sizeof(*LRU));
	if (!LRU)
		return NULL;
	LRU->capcity = capacity;
	LRU->table = baseOHTInit(element_size, element_align, sizeof(DoubleListNode*), alingof(DoubleListNode*), hash_func, compare_func);
	if (!LRU->table)
		return NULL;
	LRU->list = doubleListAlloc(element_size);
	if (!LRU->list)
		return NULL;
	return LRU;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lruCacheContains(LRUCache* LRU, void const* key)
{	
	return baseOHTFind(LRU->table, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lruCacheAddorReplace(LRUCache* LRU, void const* key, void const* replace)
{
	if (LRU->capacity <= LRU->list->length) {
		DoubleListNode* new_end = doubleListPopBack(LRU->list);
		baseOHTDelete(LRU->table, LRU->old_key);
		replace = LRU->old_key;
		LRU->old_key = getKey(new_end->data); // get hash

		if (lruCacheAdd(LRU, key) == CACHE_POLICY_ADD_ERORR)
			return CACHE_POLICY_ADD_ERORR;
		return CACHE_POLICY_ADD_REPLACE;
	}

	return lruCacheAdd(LRU, key);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lruCacheAdd(LRUCache* LRU, void const* key)
{
	void const* val = doubleListAddFront(LRU->list, key);
	if (val == NULL)
		return CACHE_POLICY_ADD_ERORR;
	void* result = baseOHTInsert(LRU->table, key, val);
	if (result == NULL)
		return CACHE_POLICY_ADD_ERORR;

	return CACHE_POLICY_ADD_NO_REPLACE;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void lruCacheFree(LRUCache* LRU)
{
	if(!baseOHTIsEmpty(LRU->table))
		baseOHTFree(LRU->table);
	doubleListFree(LRU->list);

	free(LRU);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
