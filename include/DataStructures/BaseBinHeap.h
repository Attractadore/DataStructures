#pragma once

#include "BaseFunc.h"

#include <stddef.h>

typedef struct BaseBinHeap_T BaseBinHeap;

BaseBinHeap *baseBinHeapAlloc(size_t value_size, BaseOrderFunc order_func);

void baseBinHeapFree(BaseBinHeap *heap);

void baseBinHeapPop(BaseBinHeap *heap, void *res);
bool baseBinHeapInsert(BaseBinHeap *heap, void const *value);

size_t baseBinHeapSize(BaseBinHeap const *heap);
