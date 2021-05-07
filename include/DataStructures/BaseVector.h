// This is an internal implementation header -- use Vector.h instead
#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct BaseVector_T BaseVector;

BaseVector* baseVectorAlloc(size_t value_size);
void baseVectorFree(BaseVector* vector);

void* baseVectorPushBack(BaseVector* vector, void const* value);
void baseVectorPopBack(BaseVector* vector, void* res);

void* baseVectorData(BaseVector* vector);
void const* baseVectorConstData(BaseVector const* vector);
void* baseVectorPtr(BaseVector* vector, size_t i);
void const* baseVectorConstPtr(BaseVector const* vector, size_t i);
void* baseVectorBackPtr(BaseVector* vector);
void const* baseVectorBackConstPtr(BaseVector const* vector);

void* baseVectorDisown(BaseVector* vector, size_t* size_ptr, size_t* capacity_ptr);
void baseVectorClear(BaseVector* vector);

size_t baseVectorSize(BaseVector const* vector);
size_t baseVectorValueSize(BaseVector const* vector);
