#include "BaseChainHashTable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct BaseCHTNode_T {
  struct BaseCHTNode_T *next;
  unsigned char data[];
} BaseCHTNode;

struct BaseCHT_T {
  size_t capacity, size;
  size_t key_size, key_align, value_size, value_align;
  size_t node_ptr_alloc_size, node_key_alloc_size, node_alloc_size;
  BaseHashFunc hash_func;
  BaseCompareFunc compare_func;
  BaseCHTNode **buckets;
  BaseCHTNode head;
};

static const size_t CHAIN_HASH_TABLE_BASE_CAPACITY = 32;
static const double CHAIN_HASH_TABLE_MAX_LOAD_FACTOR = 1.0;
static const double CHAIN_HASH_TABLE_MIN_LOAD_FACTOR = 0.25;

static void baseCHTnInsertAfter(BaseCHTNode *const node,
                                BaseCHTNode *const insert_after) {
  assert(node && insert_after);

  BaseCHTNode *const next = insert_after->next;
  insert_after->next = node;
  node->next = next;
}

static void *baseCHTnKeyPointer(BaseCHTNode *const node,
                                BaseCHT const *const table) {
  assert(node);

  return (unsigned char *)node + table->node_ptr_alloc_size;
}

static void *baseCHTnValuePointer(BaseCHTNode *const node,
                                  BaseCHT const *const table) {
  assert(node && table);

  return (unsigned char *)baseCHTnKeyPointer(node, table) +
         table->node_key_alloc_size;
}

static size_t baseCHTGetIdx(BaseCHT const *const table, void const *const key) {
  assert(table && key);

  return table->hash_func(key) & (table->capacity - 1);
}

static BaseCHTNode *baseCHTFindPrev(BaseCHT *const table,
                                    void const *const key) {
  assert(table && key);

  const size_t idx = baseCHTGetIdx(table, key);
  BaseCHTNode *prev = table->buckets[idx];
  if (!prev) {
    return NULL;
  }
  while (prev->next) {
    void const *const key_ptr = baseCHTnKeyPointer(prev->next, table);
    if (baseCHTGetIdx(table, key_ptr) != idx) {
      return NULL;
    }
    if (table->compare_func(key_ptr, key)) {
      return prev;
    }
    prev = prev->next;
  }

  return NULL;
}

static void baseCHTInsertInChain(BaseCHT *const table,
                                 BaseCHTNode *const node) {
  assert(table && node);

  const size_t idx = baseCHTGetIdx(table, baseCHTnKeyPointer(node, table));
  BaseCHTNode *const insert_after =
      (table->buckets[idx]) ? (table->buckets[idx]) : (&table->head);
  baseCHTnInsertAfter(node, insert_after);

  // Handle insertion of first item in chain
  if (!table->buckets[idx]) {
    table->buckets[idx] = &table->head;
    if (node->next) {
      const size_t update_idx =
          baseCHTGetIdx(table, baseCHTnKeyPointer(node->next, table));
      table->buckets[update_idx] = node;
    }
  }
}

static size_t baseCHTResize(BaseCHT *const table, size_t new_capacity) {
  assert(table);

  if (new_capacity < CHAIN_HASH_TABLE_BASE_CAPACITY) {
    new_capacity = CHAIN_HASH_TABLE_BASE_CAPACITY;
  }

  if (new_capacity == table->capacity) {
    return table->capacity;
  }

  BaseCHTNode **const new_buckets =
      realloc(table->buckets, new_capacity * sizeof(*new_buckets));
  if (!new_buckets) {
    return table->capacity;
  }
  table->buckets = new_buckets;
  table->capacity = new_capacity;
  memset(table->buckets, 0, table->capacity * sizeof(*table->buckets));

  BaseCHTNode *pop_node = table->head.next;
  table->head.next = NULL;
  while (pop_node) {
    BaseCHTNode *const insert_node = pop_node;
    pop_node = pop_node->next;
    baseCHTInsertInChain(table, insert_node);
  }

  return table->capacity;
}

static BaseCHTNode *baseCHTAllocInChain(BaseCHT *const table,
                                        void const *const key) {
  assert(table && key);

  BaseCHTNode *const node = calloc(1, table->node_alloc_size);
  if (!node) {
    return NULL;
  }
  memcpy(baseCHTnKeyPointer(node, table), key, table->key_size);
  baseCHTInsertInChain(table, node);
  table->size++;

  if (table->size >
      (size_t)((double)table->capacity * CHAIN_HASH_TABLE_MAX_LOAD_FACTOR)) {
    baseCHTResize(table, table->capacity * 2);
  }

  return node;
}

static void baseCHTDeleteInChain(BaseCHT *const table,
                                 BaseCHTNode *const delete_after) {
  assert(table && delete_after);

  BaseCHTNode *const node = delete_after->next;

  // Handle deletion of last item in chain
  const size_t idx = baseCHTGetIdx(table, baseCHTnKeyPointer(node, table));
  const size_t next_idx =
      (node->next)
          ? (baseCHTGetIdx(table, baseCHTnKeyPointer(node->next, table)))
          : (table->capacity);
  if (idx != next_idx) {
    if (node->next) {
      table->buckets[next_idx] = delete_after;
    }
    // Handle deletion of last item
    BaseCHTNode *const prev = table->buckets[idx];
    if (delete_after == prev) {
      table->buckets[idx] = NULL;
    }
  }

  delete_after->next = node->next;
  free(node);
  table->size--;

  if (table->capacity > CHAIN_HASH_TABLE_BASE_CAPACITY &&
      table->size < (size_t)((double)table->capacity *
                             CHAIN_HASH_TABLE_MIN_LOAD_FACTOR)) {
    baseCHTResize(table, table->capacity / 2);
  }
}

static size_t align(const size_t size, const size_t alignment) {
  return alignment * (size / alignment + (size % alignment != 0));
}

BaseCHT *baseCHTInit(const size_t key_size, const size_t key_align,
                     const size_t value_size, const size_t value_align,
                     const BaseHashFunc hash_func,
                     const BaseCompareFunc compare_func) {
  assert(hash_func && compare_func);

  BaseCHT *const table = calloc(1, sizeof(*table));
  if (!table) {
    return NULL;
  }
  table->capacity = CHAIN_HASH_TABLE_BASE_CAPACITY;

  table->key_size = key_size;
  table->key_align = key_align;
  table->value_size = value_size;
  table->value_align = value_align;

  table->node_ptr_alloc_size =
      align(sizeof(table->head.next), table->key_align);
  table->node_key_alloc_size = align(table->key_size, table->value_align);
  table->node_alloc_size = table->node_ptr_alloc_size +
                           table->node_key_alloc_size + table->value_size;

  table->hash_func = hash_func;
  table->compare_func = compare_func;
  table->buckets = calloc(table->capacity, sizeof(*table->buckets));
  if (!table->buckets) {
    free(table);
    return NULL;
  }

  return table;
}

static void baseCHTFreeNodes(BaseCHT *const table) {
  assert(table);

  BaseCHTNode *node = table->head.next;
  while (node) {
    BaseCHTNode *const next = node->next;
    free(node);
    node = next;
  }
}

void baseCHTFree(BaseCHT *const table) {
  if (!table) {
    return;
  }
  baseCHTFreeNodes(table);
  free(table->buckets);
  free(table);
}

void *baseCHTInsert(BaseCHT *const table, void const *const key,
                    void const *const value) {
  assert(table && key && value);

  void *value_ptr = baseCHTFind(table, key);
  if (!value_ptr) {
    BaseCHTNode *const node = baseCHTAllocInChain(table, key);
    if (!node) {
      return NULL;
    }

    value_ptr = baseCHTnValuePointer(node, table);
  }
  memcpy(value_ptr, value, table->value_size);

  return value_ptr;
}

void *baseCHTFind(BaseCHT *const table, void const *const key) {
  assert(table && key);

  BaseCHTNode *const found_prev = baseCHTFindPrev(table, key);
  if (!found_prev) {
    return NULL;
  }

  return baseCHTnValuePointer(found_prev->next, table);
}

void *baseCHTFindOrInsert(BaseCHT *const table, void const *const key,
                          void const *const value) {
  assert(table && key && value);

  void *value_ptr = baseCHTFind(table, key);
  if (!value_ptr) {
    BaseCHTNode *const node = baseCHTAllocInChain(table, key);
    if (!node) {
      return NULL;
    }

    value_ptr = baseCHTnValuePointer(node, table);
    memcpy(value_ptr, value, table->value_size);
  }

  return value_ptr;
}

void baseCHTDelete(BaseCHT *const table, void const *const key) {
  assert(table && key);

  BaseCHTNode *const found_prev = baseCHTFindPrev(table, key);
  if (!found_prev) {
    return;
  }

  baseCHTDeleteInChain(table, found_prev);
}

bool baseCHTIsEmpty(BaseCHT const *const table) {
  assert(table);

  return !table->size;
}

size_t baseCHTSize(BaseCHT const *const table) {
  assert(table);

  return table->size;
}
