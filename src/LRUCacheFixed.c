#include "BaseDoubleList.h"
#include "BaseOpenHashTable.h"
#include "BaseVector.h"
#include "LRUCache.h"

struct LRUCache_T {
	BaseOHT* table;
	//BaseVector* vector;
	DoubleList* list;
};
/*------------------------------------------------------------------------------------------------------------------------------*/
LRUCache* LRUCacheAlloc(size_t element_size, size_t element_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func)
{
	LRUCache* LRU = calloc(1, sizeof(*LRU));
	LRU->table = baseOHTInit(element_size, element_align, value_size, value_align, hash_func, compare_func);
	//LRU->vector = baseVectorAlloc(element_size);
	LRU->list = doubleListAlloc(element_size);
	return LRU;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
bool lruCacheContains(LRUCache* LRU, void const* key)
{
	DoubleListNode* tmp = (DoubleListNode*)baseOHTFind(LRU->table, key); // returns NULL if failed to find
	if (tmp) {
		doubleListMoveToFront(tmp); // may be this shouldnt be here
		return 0;
	}
	return 1; 
}
/*------------------------------------------------------------------------------------------------------------------------------*/
CachePolicyAddResult lruCacheAdd(LRUCache* LRU, void const* key, void const* replace)
{
	if (replace/*isnt enough space*/) {
		doubleListPopBack(LRU->list);
		void baseOHTDelete(LRU->table, key);
		return CACHE_POLICY_ADD_REPLACE;
	}
	void* baseOHTInsert(LRU->table, key, doubleListAddFront(LRU->list, key);
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
