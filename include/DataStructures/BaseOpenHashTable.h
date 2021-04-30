#pragma once

#include "BaseCompareFunc.h"
#include "BaseHashFunc.h"

#include <stddef.h>

typedef struct BaseOHT_T BaseOHT;

BaseOHT* baseOHTInit(size_t key_size, size_t key_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func);
void baseOHTFree(BaseOHT* table);

void* baseOHTInsert(BaseOHT* table, void const* key, void const* value);
void* baseOHTFind(BaseOHT* table, void const* key);
void* baseOHTFindOrInsert(BaseOHT* table, void const* key, void const* value);
void baseOHTDelete(BaseOHT* table, void const* key);

bool baseOHTIsEmpty(BaseOHT const* table);
size_t baseOHTSize(BaseOHT const* table);
