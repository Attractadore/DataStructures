#include "BaseOpenHashTable.c"

#include "unity.h"

#include <stdalign.h>

bool intCompare(void const *left, void const *right) {
  return strcmp(*(char const *const *)left, *(char const *const *)right) == 0;
  return *(int const *)left == *(int const *)right;
}

uint64_t intHash(void const *value) {
  char const *str = *(char const *const *)value;
  uint64_t h = 0;
  while (*str) {
    h = h * 37 + *(str++);
  }
  return h;
}

void test_Alloc() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  TEST_ASSERT_EQUAL_UINT64(OPEN_HASH_TABLE_BASE_CAPACITY, table->capacity);
  TEST_ASSERT_EQUAL_UINT64(sizeof(char *), table->key_size);
  TEST_ASSERT_EQUAL_UINT64(alignof(char *), table->key_align);
  TEST_ASSERT_EQUAL_UINT64(sizeof(int), table->value_size);
  TEST_ASSERT_EQUAL_UINT64(alignof(int), table->value_align);
  TEST_ASSERT_EQUAL_PTR(intHash, table->hash_func);
  TEST_ASSERT_EQUAL_PTR(intCompare, table->compare_func);
  TEST_ASSERT_NOT_NULL(table->buckets_data);
  baseOHTFree(table);
}

void test_Insert_not_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *find_ptr = baseOHTFind(table, &str);
  TEST_ASSERT_NULL(find_ptr);
  int *insert_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  TEST_ASSERT_EQUAL(value, *insert_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  baseOHTFree(table);
}

void test_Insert_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *find_ptr = baseOHTFind(table, &str);
  TEST_ASSERT_NULL(find_ptr);
  int *insert1_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert1_ptr);
  TEST_ASSERT_EQUAL(value, *insert1_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  value = 10;
  int *insert2_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_EQUAL_PTR(insert1_ptr, insert2_ptr);
  TEST_ASSERT_EQUAL(value, *insert2_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  baseOHTFree(table);
}

void test_Find_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *insert_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  int *find_ptr = baseOHTFind(table, &str);
  TEST_ASSERT_EQUAL_PTR(insert_ptr, find_ptr);
  baseOHTFree(table);
}

void test_Find_not_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int *find_ptr = baseOHTFind(table, &str);
  TEST_ASSERT_NULL(find_ptr);
  baseOHTFree(table);
}

void test_FindOrInsert_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *insert_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  int new_value = 10;
  int *find_ptr = baseOHTFindOrInsert(table, &str, &new_value);
  TEST_ASSERT_EQUAL_PTR(insert_ptr, find_ptr);
  TEST_ASSERT_EQUAL(value, *find_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  baseOHTFree(table);
}

void test_FindOrInsert_not_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *insert_ptr = baseOHTFindOrInsert(table, &str, &value);
  TEST_ASSERT_EQUAL(1, table->size);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  TEST_ASSERT_EQUAL(value, *insert_ptr);
  baseOHTFree(table);
}

void test_Delete_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *insert_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  baseOHTDelete(table, &str);
  int *find_ptr = baseOHTFind(table, &str);
  TEST_ASSERT_NULL(find_ptr);
  TEST_ASSERT_EQUAL(0, table->size);
  baseOHTFree(table);
}

void test_Delete_not_present() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *insert_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  char const *str2 = "Hello2";
  baseOHTDelete(table, &str2);
  int *find_ptr = baseOHTFind(table, &str);
  TEST_ASSERT_EQUAL_PTR(insert_ptr, find_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  baseOHTFree(table);
}

void test_IsEmpty_empty() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  bool is_empty = baseOHTIsEmpty(table);
  TEST_ASSERT_TRUE(is_empty);
  baseOHTFree(table);
}

void test_IsEmpty_not_empty() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "Hello";
  int value = 5;
  int *insert_ptr = baseOHTInsert(table, &str, &value);
  TEST_ASSERT_NOT_NULL(insert_ptr);
  TEST_ASSERT_EQUAL(1, table->size);
  bool is_empty = baseOHTIsEmpty(table);
  TEST_ASSERT_FALSE(is_empty);
  baseOHTFree(table);
}

void test_Size() {
  BaseOHT *table = baseOHTInit(sizeof(char *), alignof(char *), sizeof(int),
                               alignof(int), intHash, intCompare);
  TEST_ASSERT_NOT_NULL(table);
  char const *str = "0123456789";
  for (size_t i = 0; i < 10; i++) {
    size_t sz = baseOHTSize(table);
    TEST_ASSERT_EQUAL(i, sz);
    TEST_ASSERT_EQUAL(i, table->size);
    char const *ptr = str + i;
    int *insert_ptr = baseOHTInsert(table, &ptr, &i);
    TEST_ASSERT_NOT_NULL(insert_ptr);
  }
  baseOHTFree(table);
}
