/*
 * This describes a heap allocated array with automatic resizing
 */

// VECTOR_TYPE defines the type of data you want to store in a Vector.
// VECTOR_TYPE_NAME defines the name suffix of a Vector.
// If VECTOR_TYPE_NAME = Int,
// Vector becomes VectorInt, vectorAlloc becomes vectorIntAlloc.

#ifndef VECTOR_TYPE
#error Define the type to use as VECTOR_TYPE
#endif

#ifndef VECTOR_TYPE_NAME
#error Define the name of the type to use as VECTOR_TYPE_NAME
#endif

#include "BaseVector.h"

#define CONCAT_HELPER(a, b) a##b

#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define VECTOR_UPPER_NAME CONCAT(Vector, VECTOR_TYPE_NAME)
#define VECTOR_LOWER_NAME CONCAT(vector, VECTOR_TYPE_NAME)
#define VECTOR_NAME VECTOR_UPPER_NAME
#define VECTOR_UPPER_IDENTIFIER(name) CONCAT(VECTOR_UPPER_NAME, name)
#define VECTOR_LOWER_IDENTIFIER(name) CONCAT(VECTOR_LOWER_NAME, name)

typedef struct VECTOR_UPPER_IDENTIFIER(_T) VECTOR_NAME;

// vectorAlloc: Create a new Vector.
static inline VECTOR_NAME *VECTOR_LOWER_IDENTIFIER(Alloc)() {
  return (VECTOR_NAME *)baseVectorAlloc(sizeof(VECTOR_TYPE));
}

// vectorFree: Free a Vector.
static inline void VECTOR_LOWER_IDENTIFIER(Free)(VECTOR_NAME *const vector) {
  baseVectorFree((BaseVector *)vector);
}

// vectorPushBack: Append an element to a Vector.
// If an error occurs, NULL is returned.
// Otherwise, a pointer to the element appended is returned.
static inline VECTOR_TYPE *
VECTOR_LOWER_IDENTIFIER(PushBack)(VECTOR_NAME *const vector,
                                  VECTOR_TYPE const value) {
  return baseVectorPushBack((BaseVector *)vector, &value);
}

// vectorPopBack: Pop an element from the back of a Vector.
// The popped element is returned.
// If the Vector is empty, the result of this function is undefined.
static inline VECTOR_TYPE
VECTOR_LOWER_IDENTIFIER(PopBack)(VECTOR_NAME *const vector) {
  VECTOR_TYPE value;
  baseVectorPopBack((BaseVector *)vector, &value);
  return value;
}

// vectorData: Get a pointer to the beginning of a Vector's storage.
// If no storage is allocated, NULL is returned.
static inline VECTOR_TYPE *
VECTOR_LOWER_IDENTIFIER(Data)(VECTOR_NAME *const vector) {
  return baseVectorData((BaseVector *)vector);
}

// vectorConstData: Get a const pointer to the beginning of a Vector's storage.
// If no storage is allocated, NULL is returned.
static inline VECTOR_TYPE const *
VECTOR_LOWER_IDENTIFIER(ConstData)(VECTOR_NAME const *const vector) {
  return baseVectorConstData((BaseVector const *)vector);
}

// vectorPtr: Get a pointer the element at an index in a Vector.
// If the index is greater than the Vector's size, the result of this function
// is undefined.
static inline VECTOR_TYPE *
VECTOR_LOWER_IDENTIFIER(Ptr)(VECTOR_NAME *const vector, const size_t i) {
  return baseVectorPtr((BaseVector *)vector, i);
}

// vectorConstPtr: Get a const pointer the element at an index in a Vector.
// If the index is greater than the Vector's size, the result of this function
// is undefined.
static inline VECTOR_TYPE const *
VECTOR_LOWER_IDENTIFIER(ConstPtr)(VECTOR_NAME *const vector, const size_t i) {
  return baseVectorConstPtr((BaseVector const *)vector, i);
}

// vectorBackPtr: Get a pointer to the last element in a Vector.
// If the Vector is empty, the result of this function is undefined.
static inline VECTOR_TYPE *
VECTOR_LOWER_IDENTIFIER(BackPtr)(VECTOR_NAME *const vector) {
  return baseVectorBackPtr((BaseVector *)vector);
}

// vectorBackConstPtr: Get a const pointer to the last element in a Vector.
// If the Vector is empty, the result of this function is undefined.
static inline VECTOR_TYPE const *
VECTOR_LOWER_IDENTIFIER(BackConstPtr)(VECTOR_NAME *const vector) {
  return baseVectorBackConstPtr((BaseVector const *)vector);
}

// vectorDisown: Get a pointer to the beginning of a Vector's storage.
// After calling vectorDisown, the Vector no longer manages its storage,
// and free must be called on the returned pointer.
// If size_ptr/capacity_ptr is not NULL, then the current size/capacity
// is written to the location pointed to.
// If no storage is allocated, NULL is returned.
static inline VECTOR_TYPE *
VECTOR_LOWER_IDENTIFIER(Disown)(VECTOR_NAME *const vector,
                                size_t *const size_ptr,
                                size_t *const capacity_ptr) {
  return baseVectorDisown((BaseVector *)vector, size_ptr, capacity_ptr);
}

// vectorClear: Set a Vector's size to 0.
static inline void VECTOR_LOWER_IDENTIFIER(Clear)(VECTOR_NAME *const vector) {
  baseVectorClear((BaseVector *)vector);
}

// vectorSize: Get the size of a Vector.
static inline size_t
VECTOR_LOWER_IDENTIFIER(Size)(VECTOR_NAME const *const vector) {
  return baseVectorSize((BaseVector const *)vector);
}

#undef VECTOR_UPPER_IDENTIFIER
#undef VECTOR_LOWER_IDENTIFIER
#undef VECTOR_NAME
#undef VECTOR_UPPER_NAME
#undef VECTOR_LOWER_NAME

#undef CONCAT
#undef CONCAT_HELPER

#undef VECTOR_TYPE_NAME
#undef VECTOR_TYPE
