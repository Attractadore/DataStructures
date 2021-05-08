#pragma once
#include <stddef.h>

typedef struct MonoList_T MonoList;
typedef struct mononode_t MonoListNode;

MonoList* monoListAlloc(size_t key_size);
void monoListFree(MonoList* ml);
MonoListNode* monoListAddToFront(MonoList* ml, void const* value);
MonoListNode* monoListAddToBack(MonoList* ml, void const* value);
MonoListNode* monoListPopBack(MonoList* ml);
MonoListNode* monoListPopFront(MonoList* ml);
MonoListNode* monoListMoveNextToFront(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListMoveNextToBack(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListRemoveNext(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListDeleteNext(MonoList* ml, MonoListNode* prenode);
MonoListNode* monoListAppend(MonoList* ml, MonoListNode* node);
MonoListNode* monoListPrepend(MonoList* ml, MonoListNode* node);
MonoListNode* Front(MonoList* ml);
MonoListNode* Back(MonoList* ml);
unsigned char* Data(MonoList* ml);
MonoListNode const* ConstFront(MonoList const* ml);
MonoListNode const* ConstBack(MonoList const* ml);
unsigned char const* ConstData(MonoList const* ml);

