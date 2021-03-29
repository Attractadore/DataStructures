#include "BaseVector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct BaseVector_T {
    size_t elem_size;
    size_t capacity;
    size_t size;
    unsigned char* data;
};

enum {
    VECTOR_DEFAULT_CAPACITY = 128,
};

BaseVector* baseVectorAlloc(const size_t elem_size) {
    BaseVector* const vector = calloc(1, sizeof(*vector));
    if (!vector) {
        return vector;
    }

    vector->elem_size = elem_size;
    vector->capacity = VECTOR_DEFAULT_CAPACITY;
    vector->data = calloc(vector->capacity, vector->elem_size);
    if (!vector->data) {
        free(vector);
        return NULL;
    }

    return vector;
}

void baseVectorFree(BaseVector* const vector) {
    if (!vector) {
        return;
    }

    free(vector->data);
    free(vector);
}

bool baseVectorPushBack(BaseVector* const vector, void const* const elem_ptr) {
    assert(vector && elem_ptr);

    if (vector->size == vector->capacity) {
        const size_t new_capacity = vector->capacity * 2;
        void* const new_data = realloc(vector->data, new_capacity * vector->elem_size);
        if (!new_data) {
            return false;
        }
        vector->capacity = new_capacity;
        vector->data = new_data;
    }
    assert(vector->size < vector->capacity);

    memcpy(vector->data + vector->size * vector->elem_size, elem_ptr, vector->elem_size);
    vector->size++;

    return true;
}

void* baseVectorData(BaseVector* const vector) {
    assert(vector);

    return vector->data;
}

void* baseVectorDisown(BaseVector* const vector, size_t* const size_ptr, size_t* const capacity_ptr) {
    assert(vector);

    if (size_ptr) {
        *size_ptr = vector->size;
    }

    if (capacity_ptr) {
        *capacity_ptr = vector->capacity;
    }

    void* const data = vector->data;

    vector->size = 0;
    vector->capacity = 0;
    vector->data = NULL;

    return data;
}

void const* baseVectorConstData(BaseVector const* const vector) {
    assert(vector);

    return vector->data;
}

size_t baseVectorGetSize(BaseVector const* const vector) {
    assert(vector);

    return vector->size;
}
