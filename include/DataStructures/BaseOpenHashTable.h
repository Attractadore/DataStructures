#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint64_t (*BaseHashFunc)(void const* key);
typedef bool (*BaseCompareFunc)(void const* left_key, void const* right_key);

typedef struct BaseOHT_T BaseOHT;

BaseOHT* baseOHTInit(size_t key_size, size_t key_align, size_t value_size, size_t value_align, BaseHashFunc hash_func, BaseCompareFunc compare_func);
void baseOHTFree(BaseOHT* table);

void* baseOHTInsert(BaseOHT* table, void const* key, void const* value);
void* baseOHTFind(BaseOHT* table, void const* key);
void* baseOHTFindOrInsert(BaseOHT* table, void const* key, void const* value);
void baseOHTDelete(BaseOHT* table, void const* key);

bool baseOHTIsEmpty(BaseOHT const* table);
size_t baseOHTSize(BaseOHT const* table);
