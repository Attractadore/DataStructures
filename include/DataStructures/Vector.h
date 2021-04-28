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

static inline VECTOR_NAME* VECTOR_LOWER_IDENTIFIER(Alloc)() {
    return (VECTOR_NAME*) baseVectorAlloc(sizeof(VECTOR_TYPE));
}
static inline void VECTOR_LOWER_IDENTIFIER(Free)(VECTOR_NAME* const vector) {
    baseVectorFree((BaseVector*) vector);
}

static inline bool VECTOR_LOWER_IDENTIFIER(PushBack)(VECTOR_NAME* const vector, VECTOR_TYPE const elem) {
    return baseVectorPushBack((BaseVector*) vector, &elem);
}

static inline VECTOR_TYPE* VECTOR_LOWER_IDENTIFIER(Data)(VECTOR_NAME* const vector) {
    return baseVectorData((BaseVector*) vector);
}

static inline VECTOR_TYPE const* VECTOR_LOWER_IDENTIFIER(ConstData)(VECTOR_NAME const* const vector) {
    return baseVectorConstData((BaseVector const*) vector);
}

static inline VECTOR_TYPE* VECTOR_LOWER_IDENTIFIER(Disown)(VECTOR_NAME* const vector, size_t* const size_ptr, size_t* const capacity_ptr) {
    return baseVectorDisown((BaseVector*) vector, size_ptr, capacity_ptr);
}

static inline void VECTOR_LOWER_IDENTIFIER(Clear)(VECTOR_NAME* const vector) {
    baseVectorClear((BaseVector*) vector);
}

static inline size_t VECTOR_LOWER_IDENTIFIER(GetSize)(VECTOR_NAME const* const vector) {
    return baseVectorGetSize((BaseVector*) vector);
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
