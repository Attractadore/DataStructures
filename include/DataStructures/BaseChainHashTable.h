#pragma once

#include "BaseFunc.h"

#include <stddef.h>

typedef struct BaseCHT_T BaseCHT;

BaseCHT *baseCHTInit(size_t key_size, size_t key_align, size_t value_size,
                     size_t value_align, BaseHashFunc hash_func,
                     BaseCompareFunc compare_func);
void baseCHTFree(BaseCHT *table);

void *baseCHTInsert(BaseCHT *table, void const *key, void const *value);
void *baseCHTFind(BaseCHT *table, void const *key);
void *baseCHTFindOrInsert(BaseCHT *table, void const *key, void const *value);
void baseCHTDelete(BaseCHT *table, void const *key);

size_t baseCHTSize(BaseCHT const *table);
