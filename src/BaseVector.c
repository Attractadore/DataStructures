#include "BaseVector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct BaseVector_T {
  size_t value_size;
  size_t capacity;
  size_t size;
  unsigned char *data;
};

enum {
  VECTOR_DEFAULT_CAPACITY = 16,
};

BaseVector *baseVectorAlloc(const size_t value_size) {
  BaseVector *const vector = calloc(1, sizeof(*vector));
  if (!vector) {
    return vector;
  }

  vector->value_size = value_size;

  return vector;
}

void baseVectorFree(BaseVector *const vector) {
  if (!vector) {
    return;
  }

  free(vector->data);
  free(vector);
}

void *baseVectorPushBack(BaseVector *const vector, void const *const value) {
  assert(vector && value);

  if (!vector->data || vector->size == vector->capacity) {
    const size_t new_capacity =
        (vector->capacity) ? (vector->capacity * 2) : (VECTOR_DEFAULT_CAPACITY);
    void *const new_data =
        realloc(vector->data, new_capacity * vector->value_size);
    if (!new_data) {
      return NULL;
    }
    vector->capacity = new_capacity;
    vector->data = new_data;
  }
  assert(vector->size < vector->capacity);

  vector->size++;
  void *const back_ptr = baseVectorBackPtr(vector);
  memcpy(back_ptr, value, vector->value_size);

  return back_ptr;
}

void baseVectorPopBack(BaseVector *const vector, void *const res) {
  assert(vector && vector->size);

  if (res) {
    memcpy(res, baseVectorBackConstPtr(vector), vector->value_size);
  }

  vector->size--;
}

void *baseVectorData(BaseVector *const vector) {
  assert(vector);

  return vector->data;
}

void const *baseVectorConstData(BaseVector const *const vector) {
  assert(vector);

  return vector->data;
}

void *baseVectorPtr(BaseVector *const vector, const size_t i) {
  assert(vector && i < vector->size);

  return vector->data + i * vector->value_size;
}

void const *baseVectorConstPtr(BaseVector const *const vector, const size_t i) {
  assert(vector && i < vector->size);

  return vector->data + i * vector->value_size;
}

void *baseVectorBackPtr(BaseVector *const vector) {
  assert(vector && vector->size);

  return baseVectorPtr(vector, vector->size - 1);
}

void const *baseVectorBackConstPtr(BaseVector const *const vector) {
  assert(vector && vector->size);

  return baseVectorConstPtr(vector, vector->size - 1);
}

void *baseVectorDisown(BaseVector *const vector, size_t *const size_ptr,
                       size_t *const capacity_ptr) {
  assert(vector);

  if (size_ptr) {
    *size_ptr = vector->size;
  }

  if (capacity_ptr) {
    *capacity_ptr = vector->capacity;
  }

  void *const data = vector->data;

  vector->size = 0;
  vector->capacity = 0;
  vector->data = NULL;

  return data;
}

void baseVectorClear(BaseVector *const vector) {
  assert(vector);

  vector->size = 0;
}

size_t baseVectorSize(BaseVector const *const vector) {
  assert(vector);

  return vector->size;
}

size_t baseVectorValueSize(BaseVector const *const vector) {
  assert(vector);

  return vector->value_size;
}
