#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

typedef struct MonoList_T MonoList;
typedef struct mononode_t MonoListNode;

MonoList* monoListAlloc(size_t key_size);
void monoListFree(MonoList* ml);
MonoListNode* monoListAddFront(MonoList* ml, void const* value);
MonoListNode* monoListPopBack(MonoList* ml);
//next two functions receive the node previous to the node being operated with
MonoListNode* monoListMoveToFront(MonoList* ml, MonoListNode* node);
MonoListNode* monoListRemove(MonoList* ml, MonoListNode* node);
