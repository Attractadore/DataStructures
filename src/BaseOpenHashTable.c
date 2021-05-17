#include "BaseOpenHashTable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  OPEN_HASH_TABLE_EMPTY,
  OPEN_HASH_TABLE_IN_USE,
  OPEN_HASH_TABLE_DELETED,
} BaseOHTState;

struct BaseOHT_T {
  size_t capacity;
  size_t size;
  size_t key_size, key_align, value_size, value_align;
  size_t key_alloc_size, bucket_alloc_size;
  BaseHashFunc hash_func;
  BaseCompareFunc compare_func;
  unsigned char *buckets_data;
};

const size_t OPEN_HASH_TABLE_BASE_CAPACITY = 32;
const double OPEN_HASH_TABLE_MAX_LOAD_FACTOR = 0.5;
const double OPEN_HASH_TABLE_MIN_LOAD_FACTOR = 0.1;

static unsigned char *baseOHTKeyPointer(BaseOHT *const table,
                                        const size_t idx) {
  assert(table);

  return table->buckets_data + idx * table->bucket_alloc_size;
}

static unsigned char *baseOHTValuePointer(BaseOHT *const table,
                                          const size_t idx) {
  assert(table);

  return baseOHTKeyPointer(table, idx) + table->key_alloc_size;
}

static unsigned char *baseOHTFlagPointer(BaseOHT *const table,
                                         const size_t idx) {
  assert(table);

  return baseOHTValuePointer(table, idx) + table->value_size;
}

static BaseOHTState baseOHTGetFlag(BaseOHT *const table, const size_t idx) {
  assert(table);

  return *baseOHTFlagPointer(table, idx);
}

static void baseOHTSetFlag(BaseOHT *const table, const size_t idx,
                           const BaseOHTState new_state) {
  assert(table);

  *baseOHTFlagPointer(table, idx) = new_state;
}

static size_t baseOHTIdx(BaseOHT const *const table, const uint64_t hash,
                         const size_t i) {
  assert(table);

  return (hash + i) & (table->capacity - 1);
}

static size_t baseOHTFindInUseIdx(BaseOHT *const table, void const *const key) {
  assert(table && key);

  const uint64_t hash = table->hash_func(key);

  for (size_t i = 0; i < table->capacity; i++) {
    const size_t idx = baseOHTIdx(table, hash, i);
    const BaseOHTState current_state = baseOHTGetFlag(table, idx);
    void const *const current_key_ptr = baseOHTKeyPointer(table, idx);
    if (current_state == OPEN_HASH_TABLE_IN_USE &&
        table->compare_func(current_key_ptr, key)) {
      return idx;
    }
    if (current_state == OPEN_HASH_TABLE_EMPTY) {
      break;
    }
  }

  return table->capacity;
}

static size_t baseOHTFindInsertIdx(BaseOHT *const table,
                                   void const *const key) {
  assert(table && key);

  const uint64_t hash = table->hash_func(key);

  for (size_t i = 0; i < table->size + 1; i++) {
    const size_t idx = baseOHTIdx(table, hash, i);
    const BaseOHTState current_state = baseOHTGetFlag(table, idx);
    if (current_state != OPEN_HASH_TABLE_IN_USE) {
      return idx;
    }
  }

  assert(!"Failed to find insert pos");

  return table->capacity;
}

typedef struct {
  size_t idx;
  BaseOHTState state;
} IdxAndState;

static IdxAndState baseOHTFindInsertOrInUseIdx(BaseOHT *const table,
                                               void const *const key) {
  assert(table && key);

  const uint64_t hash = table->hash_func(key);

  IdxAndState idx_and_state = {
      .idx = table->capacity,
      .state = OPEN_HASH_TABLE_EMPTY,
  };
  for (size_t i = 0; i < table->capacity; i++) {
    const size_t idx = baseOHTIdx(table, hash, i);
    const BaseOHTState current_state = baseOHTGetFlag(table, idx);
    void const *const current_key_ptr = baseOHTKeyPointer(table, idx);
    if (current_state == OPEN_HASH_TABLE_IN_USE) {
      if (table->compare_func(current_key_ptr, key)) {
        return (IdxAndState){
            .idx = idx,
            .state = OPEN_HASH_TABLE_IN_USE,
        };
      }

    } else {
      if (idx_and_state.idx == table->capacity) {
        idx_and_state = (IdxAndState){
            .idx = idx,
            .state = current_state,
        };
        if (current_state == OPEN_HASH_TABLE_EMPTY) {
          break;
        }
      }
    }
  }

  assert(idx_and_state.idx < table->capacity);

  return idx_and_state;
}

static void *baseOHTInsertAt(BaseOHT *const table, const size_t insert_idx,
                             void const *const key, void const *const value) {
  assert(table && key && value);
  assert(baseOHTGetFlag(table, insert_idx) != OPEN_HASH_TABLE_IN_USE);

  baseOHTSetFlag(table, insert_idx, OPEN_HASH_TABLE_IN_USE);
  void *const key_ptr = baseOHTKeyPointer(table, insert_idx);
  void *const value_ptr = baseOHTValuePointer(table, insert_idx);
  memcpy(key_ptr, key, table->key_size);
  memcpy(value_ptr, value, table->value_size);
  table->size++;
  return value_ptr;
}

static size_t max(const size_t a, const size_t b) {
  return (a > b) ? (a) : (b);
}

static size_t align(const size_t size, const size_t alignment) {
  return alignment * (size / alignment + (size % alignment != 0));
}

BaseOHT *baseOHTInit(const size_t key_size, const size_t key_align,
                     const size_t value_size, const size_t value_align,
                     const BaseHashFunc hash_func,
                     const BaseCompareFunc compare_func) {
  assert(hash_func && compare_func);

  BaseOHT *const table = calloc(1, sizeof(*table));
  if (!table) {
    return NULL;
  }
  table->capacity = OPEN_HASH_TABLE_BASE_CAPACITY;

  table->key_size = key_size;
  table->key_align = key_align;
  table->value_size = value_size;
  table->value_align = value_align;
  table->key_alloc_size = align(table->key_size, table->value_align),
  table->bucket_alloc_size =
      align(table->key_alloc_size + table->value_size + 1,
            max(table->key_align, table->value_align));

  table->hash_func = hash_func;
  table->compare_func = compare_func,

  table->buckets_data = calloc(table->capacity, table->bucket_alloc_size);
  if (!table->buckets_data) {
    free(table);
    return NULL;
  }

  return table;
}

void baseOHTFree(BaseOHT *const table) {
  if (!table) {
    return;
  }
  assert(table);
  free(table->buckets_data);
  free(table);
}

static bool baseOHTResize(BaseOHT *const table, size_t new_capacity) {
  assert(table);

  if (new_capacity < OPEN_HASH_TABLE_BASE_CAPACITY) {
    new_capacity = OPEN_HASH_TABLE_BASE_CAPACITY;
  }

  if (new_capacity == table->capacity) {
    return table->capacity;
  }

  BaseOHT new_table = *table;
  new_table.size = 0;
  new_table.capacity = new_capacity;
  new_table.buckets_data =
      calloc(new_table.capacity, new_table.bucket_alloc_size);
  if (!new_table.buckets_data) {
    return false;
  }

  for (size_t i = 0; i < table->capacity; i++) {
    if (baseOHTGetFlag(table, i) == OPEN_HASH_TABLE_IN_USE) {
      void const *const key_ptr = baseOHTKeyPointer(table, i);
      void const *const value_ptr = baseOHTValuePointer(table, i);
      const size_t insert_idx = baseOHTFindInsertIdx(&new_table, key_ptr);
      baseOHTInsertAt(&new_table, insert_idx, key_ptr, value_ptr);
    }
  }

  assert(new_table.size == table->size);

  free(table->buckets_data);
  memcpy(table, &new_table, sizeof(new_table));

  return true;
}

void *baseOHTFind(BaseOHT *const table, void const *const key) {
  assert(table && key);

  const size_t idx = baseOHTFindInUseIdx(table, key);
  if (idx >= table->capacity) {
    return NULL;
  }

  return baseOHTValuePointer(table, idx);
}

void *baseOHTInsert(BaseOHT *const table, void const *const key,
                    void const *const value) {
  assert(table && key && value);

  if (table->size > table->capacity * OPEN_HASH_TABLE_MAX_LOAD_FACTOR) {
    if (!baseOHTResize(table, table->capacity * 2)) {
      return NULL;
    }
  }

  const IdxAndState idx_and_state = baseOHTFindInsertOrInUseIdx(table, key);
  if (idx_and_state.state == OPEN_HASH_TABLE_IN_USE) {
    void *const value_ptr = baseOHTValuePointer(table, idx_and_state.idx);
    memcpy(value_ptr, value, table->value_size);
    return value_ptr;
  }

  return baseOHTInsertAt(table, idx_and_state.idx, key, value);
}

void *baseOHTFindOrInsert(BaseOHT *const table, void const *const key,
                          void const *const value) {
  assert(table && key && value);

  if (table->size > table->capacity * OPEN_HASH_TABLE_MAX_LOAD_FACTOR) {
    if (!baseOHTResize(table, table->capacity * 2)) {
      return NULL;
    }
  }

  const IdxAndState idx_and_state = baseOHTFindInsertOrInUseIdx(table, key);
  if (idx_and_state.state == OPEN_HASH_TABLE_IN_USE) {
    return baseOHTValuePointer(table, idx_and_state.idx);
  }

  return baseOHTInsertAt(table, idx_and_state.idx, key, value);
}

void baseOHTDelete(BaseOHT *const table, void const *const key) {
  assert(table && key);

  const size_t idx = baseOHTFindInUseIdx(table, key);
  if (idx >= table->capacity) {
    return;
  }

  baseOHTSetFlag(table, idx, OPEN_HASH_TABLE_DELETED);
  table->size--;
}

bool baseOHTIsEmpty(BaseOHT const *const table) {
  assert(table);

  return !table->size;
}

size_t baseOHTSize(BaseOHT const *const table) {
  assert(table);

  return table->size;
}
