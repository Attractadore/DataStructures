// This is an internal implementation header -- use Vector.h instead
#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct BaseVector_T BaseVector;

BaseVector* baseVectorAlloc(size_t elem_size);
void baseVectorFree(BaseVector* vector);

bool baseVectorPushBack(BaseVector* vector, void const* elem_ptr);

void* baseVectorData(BaseVector* vector);
void const* baseVectorConstData(BaseVector const* vector);
void* baseVectorDisown(BaseVector* vector, size_t* size_ptr, size_t* capacity_ptr);
void baseVectorClear(BaseVector* vector);

size_t baseVectorGetSize(BaseVector const* vector);
