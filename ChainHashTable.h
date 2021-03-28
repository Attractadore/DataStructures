#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint64_t (*ChainHashFunc)(void const* key);
typedef bool (*CompareFunc)(void const* left_key, void const* right_key);

typedef struct ChainHashTable_T ChainHashTable;

ChainHashTable* chainHashTableInit(size_t key_size, size_t key_align, size_t value_size, size_t value_align, ChainHashFunc hash_func, CompareFunc compare_func);
void chainHashTableFree(ChainHashTable* table);

void* chainHashTableInsert(ChainHashTable* table, void const* key, void const* value);
void* chainHashTableFind(ChainHashTable const* table, void const* key);
void chainHashTableDelete(ChainHashTable* table, void const* key);

bool chainHashTableIsEmpty(ChainHashTable const* table);
size_t chainHashTableSize(ChainHashTable const* table);
