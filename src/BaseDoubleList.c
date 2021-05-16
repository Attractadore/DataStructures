// BaseDoubleList realisation
#include "BaseDoubleList.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DoubleListNode_T {
  DoubleListNode *next;
  DoubleListNode *prev;
  unsigned char data[];
};

struct DoubleList_T {
  DoubleListNode *start;
  DoubleListNode *end;
  size_t key_size;
  size_t length;
};

/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleList *doubleListAlloc(size_t key_size) {
  DoubleList *list = calloc(1, sizeof(DoubleList));
  if (!list)
    return NULL;
  list->key_size = key_size;

  return list;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode *doubleListAddFront(DoubleList *list, void const *value) {
  DoubleListNode *node = calloc(1, sizeof(DoubleListNode) + list->key_size);
  if (!node)
    return NULL;
  memcpy(node->data, value, list->key_size);

  return doubleListPrepend(list, node);
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode *doubleListPrepend(DoubleList *list, DoubleListNode *node) {
  assert(node);
  if (list->start == NULL) {
    list->start = node;
    list->end = list->start;
  } else {
    list->start->prev = node;
    node->next = list->start;
    list->start = node;
  }
  list->length++; // update cur_size
  return list->start;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode *doubleListPopFront(DoubleList *list) {
  assert(list && list->start);

  doubleListDelete(list, list->start);
  return list->start;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode *doubleListPopBack(DoubleList *list) {
  assert(list && list->end && "DoubleList is already empty!");

  doubleListDelete(list, list->end);
  return list->end;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode *doubleListMoveToFront(DoubleList *list, DoubleListNode *node) {
  doubleListPrepend(list, doubleListRemove(list, node));

  return list->start;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode *doubleListRemove(DoubleList *list, DoubleListNode *node) {
  assert(node && "Error: node == nullptr");

  if (node->prev) {
    node->prev->next = node->next;
  } else {
    list->start = node->next;
  }

  if (node->next) {
    node->next->prev = node->prev;
  } else {
    list->end = node->prev;
  }

  node->prev = NULL;
  node->next = NULL;
  list->length--;

  return node;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListDelete(DoubleList *list, DoubleListNode *node) {
  assert(list && node);
  free(doubleListRemove(list, node));
}
/*------------------------------------------------------------------------------------------------------------------------------*/
size_t doubleListSize(DoubleList const *list) {
  assert(list);

  return list->length;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
void const *doubleListNodeConstData(DoubleListNode const *node) {
  assert(node);

  return node->data;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
DoubleListNode const *doubleListConstBack(DoubleList const *list) {
  assert(list);

  return list->end;
}
/*------------------------------------------------------------------------------------------------------------------------------*/
size_t doubleListItemSize(DoubleList const *list) { return list->key_size; }
/*------------------------------------------------------------------------------------------------------------------------------*/
void doubleListFree(DoubleList *list) {
  if (list) {
    DoubleListNode *tmp = list->start;
    DoubleListNode *tmp_next;
    while (tmp) {
      tmp_next = tmp->next;
      free(tmp);
      tmp = tmp_next;
    }
    free(list);
  }
}
