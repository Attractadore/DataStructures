#include "BaseBinHeap.h"
#include "BaseVector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct BaseBinHeap_T {
  BaseOrderFunc order_func;
  BaseVector *data;
};

BaseBinHeap *baseBinHeapAlloc(const size_t value_size,
                              const BaseOrderFunc order_func) {
  assert(value_size && order_func);

  BaseBinHeap *const heap = calloc(1, sizeof(*heap));
  if (!heap) {
    return NULL;
  }

  heap->order_func = order_func;
  heap->data = baseVectorAlloc(value_size);
  if (!heap->data) {
    free(heap);
    return NULL;
  }

  return heap;
}

void baseBinHeapFree(BaseBinHeap *const heap) {
  if (!heap) {
    return;
  }

  baseVectorFree(heap->data);
  free(heap);
}

static size_t binHeapParent(const size_t i) {
  assert(i > 0);

  return (i - 1) / 2;
}

static size_t binHeapLeft(const size_t i) { return i * 2 + 1; }

static size_t binHeapRight(const size_t i) { return i * 2 + 2; }

static void memswap(void *const restrict left, void *const restrict right,
                    const size_t sz) {
  assert(left && right);

  unsigned char *const left_byte = left;
  unsigned char *const right_byte = right;
  for (size_t i = 0; i < sz; i++) {
    const unsigned char temp = left_byte[i];
    left_byte[i] = right_byte[i];
    right_byte[i] = temp;
  }
}

static void baseBinHeapHeapifyImp(BaseBinHeap *const heap, size_t i) {
  assert(heap);

  const size_t size = baseBinHeapSize(heap);
  void *i_ptr = baseVectorPtr(heap->data, i);
  while (true) {
    const size_t l = binHeapLeft(i);
    const size_t r = binHeapRight(i);
    size_t m = i;
    void *m_ptr = i_ptr;

    if (l < size &&
        heap->order_func(m_ptr, baseVectorPtr(heap->data, l)) <= 0) {
      m = l;
      m_ptr = baseVectorPtr(heap->data, l);
    }
    if (r < size &&
        heap->order_func(m_ptr, baseVectorPtr(heap->data, r)) <= 0) {
      m = r;
      m_ptr = baseVectorPtr(heap->data, r);
    }
    if (m == i) {
      return;
    }

    memswap(i_ptr, m_ptr, baseVectorValueSize(heap->data));
    i = m;
    i_ptr = m_ptr;
  }
}

static void baseBinHeapHeapify(BaseBinHeap *const heap) {
  assert(heap);

  if (!baseVectorSize(heap->data)) {
    return;
  }

  baseBinHeapHeapifyImp(heap, 0);
}

void baseBinHeapPop(BaseBinHeap *const heap, void *const res) {
  assert(heap && baseBinHeapSize(heap));

  const size_t value_size = baseVectorValueSize(heap->data);
  if (res) {
    memcpy(res, baseVectorConstData(heap->data), value_size);
  }
  baseVectorPopBack(heap->data, baseVectorData(heap->data));

  baseBinHeapHeapify(heap);
}

bool baseBinHeapInsert(BaseBinHeap *const heap, void const *const value) {
  assert(heap && value);

  if (!baseVectorPushBack(heap->data, value)) {
    return false;
  }

  size_t i = baseVectorSize(heap->data) - 1;
  void *i_ptr = baseVectorBackPtr(heap->data);
  while (i) {
    const size_t p = binHeapParent(i);
    void *const p_ptr = baseVectorPtr(heap->data, p);
    if (heap->order_func(value, p_ptr) <= 0) {
      break;
    }

    memswap(i_ptr, p_ptr, baseVectorValueSize(heap->data));

    i = p;
    i_ptr = p_ptr;
  }

  return true;
}

size_t baseBinHeapSize(BaseBinHeap const *const heap) {
  assert(heap);

  return baseVectorSize(heap->data);
}
