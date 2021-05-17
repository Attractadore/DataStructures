/*
 * This describes a binary max heap
 */

// BIN_HEAP_TYPE defines the type of data you want to store in a BinHeap.
// BIN_HEAP_TYPE_NAME defines the name suffix of a BinHeap.
// If BIN_HEAP_TYPE_NAME = Int,
// BinHeap becomes BinHeapInt, binHeapAlloc becomes binHeapIntAlloc.

#ifndef BIN_HEAP_TYPE
#error Define the type to use as BIN_HEAP_TYPE
#endif

#ifndef BIN_HEAP_TYPE_NAME
#error Define the name of the type to use as BIN_HEAP_TYPE_NAME
#endif

#include "BaseBinHeap.h"

#define CONCAT_HELPER(a, b) a##b

#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define BIN_HEAP_UPPER_NAME CONCAT(BinHeap, BIN_HEAP_TYPE_NAME)
#define BIN_HEAP_LOWER_NAME CONCAT(binHeap, BIN_HEAP_TYPE_NAME)
#define BIN_HEAP_NAME BIN_HEAP_UPPER_NAME
#define BIN_HEAP_UPPER_IDENTIFIER(name) CONCAT(BIN_HEAP_UPPER_NAME, name)
#define BIN_HEAP_LOWER_IDENTIFIER(name) CONCAT(BIN_HEAP_LOWER_NAME, name)

typedef struct BIN_HEAP_UPPER_IDENTIFIER(_T) BIN_HEAP_NAME;

#define BIN_HEAP_ORDER_FUNC BIN_HEAP_UPPER_IDENTIFIER(OrderFunc)

typedef int (*BIN_HEAP_ORDER_FUNC)(BIN_HEAP_TYPE const *,
                                   BIN_HEAP_TYPE const *);

// binHeapAlloc: Create a new BinHeap.
static inline BIN_HEAP_NAME *
BIN_HEAP_LOWER_IDENTIFIER(Alloc)(const BIN_HEAP_ORDER_FUNC order_func) {
  return (BIN_HEAP_NAME *)baseBinHeapAlloc(sizeof(BIN_HEAP_TYPE),
                                           (BaseOrderFunc)order_func);
}

// binHeapFree: Free a BinHeap.
static inline void BIN_HEAP_LOWER_IDENTIFIER(Free)(BIN_HEAP_NAME *const heap) {
  baseBinHeapFree((BaseBinHeap *)heap);
}

// binHeapPop: Remove the max element from a BinHeap and return it.
// If the BinHeap is empty, the result is undefined.
static inline BIN_HEAP_TYPE
BIN_HEAP_LOWER_IDENTIFIER(Pop)(BIN_HEAP_NAME *const heap) {
  BIN_HEAP_TYPE value;
  baseBinHeapPop((BaseBinHeap *)heap, &value);
  return value;
}

// binHeapInsert: Add an element to a BinHeap.
// If an error occured, return false.
// Otherwise, return true.
static inline bool
BIN_HEAP_LOWER_IDENTIFIER(Insert)(BIN_HEAP_NAME *const heap,
                                  BIN_HEAP_TYPE const value) {
  return baseBinHeapInsert((BaseBinHeap *)heap, &value);
}

// binHeapSize: Get the size of a BinHeap.
static inline size_t
BIN_HEAP_LOWER_IDENTIFIER(Size)(BIN_HEAP_NAME const *const heap) {
  return baseBinHeapSize((BaseBinHeap const *)heap);
}

#undef BIN_HEAP_UPPER_IDENTIFIER
#undef BIN_HEAP_LOWER_IDENTIFIER
#undef BIN_HEAP_NAME
#undef BIN_HEAP_UPPER_NAME
#undef BIN_HEAP_LOWER_NAME

#undef CONCAT
#undef CONCAT_HELPER

#undef BIN_HEAP_TYPE_NAME
#undef BIN_HEAP_TYPE
