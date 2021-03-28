#include "ChainHashTable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct ChainHashTableNode_T {
    struct ChainHashTableNode_T* next;
    unsigned char data[];
} ChainHashTableNode;

struct ChainHashTable_T {
    size_t capacity;
    size_t size;
    size_t key_size;
    size_t key_align;
    size_t value_size;
    size_t value_align;
    ChainHashFunc hash_func;
    CompareFunc compare_func;
    ChainHashTableNode* buckets;
};

const size_t CHAIN_HASH_TABLE_BASE_CAPACITY = 10;
const double CHAIN_HASH_TABLE_MAX_LOAD_FACTOR = 1.0;
const double CHAIN_HASH_TABLE_MIN_LOAD_FACTOR = 0.25;
const double CHAIN_HASH_TABLE_GROW_FACTOR = 2.0;
const double CHAIN_HASH_TABLE_SHRINK_FACTOR = 0.5;

bool chainHashTableIsValid(ChainHashTable const* const table) {
#ifndef NDEBUG
    return true;
#else
    if (!table) {
        return false;
    }

    if (!table->key_size) {
        return false;
    }
    if (!table->key_align) {
        return false;
    }
    if (table->key_size < table->key_align) {
        return false;
    }
    if (!table->value_size) {
        return false;
    }
    if (!table->value_align) {
        return false;
    }
    if (table->value_size < table->value_align) {
        return false;
    }
    if (!table->hash_func) {
        return false;
    }
    if (!table->compare_func) {
        return false;
    }
    if (!table->buckets) {
        return false;
    }
    if (table->capacity < CHAIN_HASH_TABLE_BASE_CAPACITY) {
        return false;
    }

    size_t items_length = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        ChainHashTableNode* node = table->buckets[i].next;
        while (node) {
            node = node->next;
            items_length++;
        }
    }
    if (items_length != table->size) {
        return false;
    }

    return true;
#endif
}

size_t chainHashTableKeyAllocSize(ChainHashTable const* const table) {
    assert(chainHashTableIsValid(table));

    if (table->value_align > table->key_size) {
        return table->value_align;
    }
    return table->key_size;
}

size_t chainHashTableValueAllocSize(ChainHashTable const* const table) {
    assert(chainHashTableIsValid(table));

    return table->value_size;
}

size_t chainHashTableKeyValueAllocSize(ChainHashTable const* const table) {
    assert(chainHashTableIsValid(table));

    return chainHashTableKeyAllocSize(table) + chainHashTableValueAllocSize(table);
}

void* chainHashTableNodeKeyPointer(ChainHashTableNode* const node) {
    assert(node);

    return node->data;
}

void const* chainHashTableNodeKeyConstPointer(ChainHashTableNode const* const node) {
    assert(node);

    return node->data;
}

void* chainHashTableNodeValuePointer(ChainHashTableNode* const node, ChainHashTable const* const table) {
    assert(node);
    assert(chainHashTableIsValid(table));

    return node->data + chainHashTableKeyAllocSize(table);
}

void const* chainHashTableNodeValueConstPointer(ChainHashTableNode const* const node, ChainHashTable const* const table) {
    assert(node);
    assert(chainHashTableIsValid(table));

    return node->data + chainHashTableKeyAllocSize(table);
}

void clearBuckets(ChainHashTableNode* const buckets, const size_t num_buckets) {
    assert(buckets);

    for (size_t i = 0; i < num_buckets; i++) {
        ChainHashTableNode* node = buckets[i].next;
        while (node) {
            ChainHashTableNode* next = node->next;
            free(node);
            node = next;
        }
        buckets[i].next = NULL;
    }
}

void chainHashTableClear(ChainHashTable* const table) {
    assert(chainHashTableIsValid(table));

    clearBuckets(table->buckets, table->capacity);
    table->size = 0;
}

ChainHashTable* chainHashTableInit(const size_t key_size, const size_t key_align,
                                   const size_t value_size, const size_t value_align,
                                   const ChainHashFunc hash_func, const CompareFunc compare_func) {
    assert(key_size && key_align && value_size && value_align && key_align <= key_size && value_align <= value_size);
    assert(hash_func && compare_func);

    ChainHashTable* const table = calloc(1, sizeof(*table));
    if (!table) {
        return NULL;
    }

    table->capacity = CHAIN_HASH_TABLE_BASE_CAPACITY;
    table->key_size = key_size;
    table->key_align = key_align;
    table->value_size = value_size;
    table->value_align = value_align;
    table->hash_func = hash_func;
    table->compare_func = compare_func;
    table->buckets = calloc(table->capacity, sizeof(*table->buckets));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    return table;
}

void chainHashTableFree(ChainHashTable* const table) {
    if (!table) {
        return;
    }
    assert(chainHashTableIsValid(table));
    chainHashTableClear(table);
    free(table->buckets);
    free(table);
}

void* chainHashTableInsertNoResize(ChainHashTable* const table, void const* key, void const* value);

size_t chainHashTableResize(ChainHashTable* const table, size_t new_capacity) {
    if (new_capacity < CHAIN_HASH_TABLE_BASE_CAPACITY) {
        new_capacity = CHAIN_HASH_TABLE_BASE_CAPACITY;
    }

    if (new_capacity == table->capacity) {
        return table->capacity;
    }

    ChainHashTableNode* const new_buckets = calloc(new_capacity, sizeof(*new_buckets));
    if (!new_buckets) {
        return table->capacity;
    }

    ChainHashTableNode* const old_buckets = table->buckets;
    const size_t old_capacity = table->capacity;
    const size_t old_size = table->size;

    table->buckets = new_buckets;
    table->capacity = new_capacity;
    table->size = 0;

    for (size_t i = 0; i < old_capacity; i++) {
        ChainHashTableNode const* node = old_buckets[i].next;
        while (node) {
            void const* const key_ptr = chainHashTableNodeKeyConstPointer(node);
            void const* const value_ptr = chainHashTableNodeValueConstPointer(node, table);
            if (!chainHashTableInsertNoResize(table, key_ptr, value_ptr)) {
                chainHashTableClear(table);
                free(new_buckets);
                table->buckets = old_buckets;
                table->capacity = old_capacity;
                table->size = old_size;
                return table->capacity;
            }
            node = node->next;
        }
    }

    assert(table->size == old_size);

    clearBuckets(old_buckets, old_capacity);
    free(old_buckets);

    return table->capacity;
}

// Return pointer to node after which item with key should be inserted
// If key is already preset, node->next != NULL
// Otherwise, node->next == NULL
ChainHashTableNode* chainHashTableFindInsertPos(ChainHashTable const* const table, void const* const key) {
    assert(chainHashTableIsValid(table) && key);

    const size_t bucket_idx = table->hash_func(key) % table->capacity;

    ChainHashTableNode* insert_after = &table->buckets[bucket_idx];
    while (insert_after->next) {
        void const* const key_ptr = chainHashTableNodeKeyPointer(insert_after->next);
        if (table->compare_func(key_ptr, key)) {
            break;
        }
        insert_after = insert_after->next;
        assert(insert_after);
    }
    return insert_after;
}

void* chainHashTableInsertNoResize(ChainHashTable* const table, void const* const key, void const* const value) {
    assert(chainHashTableIsValid(table) && key && value);

    ChainHashTableNode* const node_prev = chainHashTableFindInsertPos(table, key);
    ChainHashTableNode* node = node_prev->next;
    if (!node) {
        node = calloc(1, sizeof(*node) + chainHashTableKeyValueAllocSize(table));
        if (!node) {
            return NULL;
        }
        node_prev->next = node;
        table->size++;
    }

    memcpy(chainHashTableNodeKeyPointer(node), key, table->key_size);
    void* const value_ptr = chainHashTableNodeValuePointer(node, table);
    memcpy(value_ptr, value, table->value_size);

    return value_ptr;
}

void* chainHashTableInsert(ChainHashTable* const table, void const* const key, void const* const value) {
    assert(chainHashTableIsValid(table) && key && value);

    if (table->size > table->capacity * CHAIN_HASH_TABLE_MAX_LOAD_FACTOR) {
        chainHashTableResize(table, table->capacity * CHAIN_HASH_TABLE_GROW_FACTOR);
    }

    return chainHashTableInsertNoResize(table, key, value);
}

void* chainHashTableFind(ChainHashTable const* const table, void const* const key) {
    assert(chainHashTableIsValid(table) && key);

    ChainHashTableNode* const node = chainHashTableFindInsertPos(table, key)->next;
    if (!node) {
        return NULL;
    }
    return chainHashTableNodeValuePointer(node, table);
}

void chainHashTableDeleteNoResize(ChainHashTable* const table, void const* const key) {
    assert(chainHashTableIsValid(table) && key);

    ChainHashTableNode* const node_prev = chainHashTableFindInsertPos(table, key);
    ChainHashTableNode* const node = node_prev->next;
    if (!node) {
        return;
    }

    node_prev->next = node->next;
    free(node);
    table->size--;
}

void chainHashTableDelete(ChainHashTable* const table, void const* const key) {
    assert(chainHashTableIsValid(table) && key);

    chainHashTableDeleteNoResize(table, key);

    if (table->capacity > CHAIN_HASH_TABLE_BASE_CAPACITY &&
        table->size < table->capacity * CHAIN_HASH_TABLE_MIN_LOAD_FACTOR) {
        chainHashTableResize(table, table->capacity * CHAIN_HASH_TABLE_SHRINK_FACTOR);
    }
}

bool chainHashTableIsEmpty(ChainHashTable const* const table) {
    assert(chainHashTableIsValid(table));

    return !table->size;
}

size_t chainHashTableSize(ChainHashTable const* const table) {
    assert(chainHashTableIsValid(table));

    return table->size;
}
