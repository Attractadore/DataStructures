#include "BaseMonoList.c"

#include "unity.h"


void test_monoListAlloc() {//0
    MonoList* l = monoListAlloc(sizeof(int));
    TEST_ASSERT_NOT_NULL(l);
    monoListFree(l);
}

void test_monoListFree() {//0
    MonoList* l = monoListAlloc(sizeof(int));
    monoListFree(l);
    TEST_ASSERT_EMPTY(l);
}

void test_monoListFront_one() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monolistAddToFront(&val1);
    node = monoListFront(l);
    TEST_ASSERT_PTR_EQUAL(front, node);
    monoListFree(l);
}

void test_monoListConstFront_one() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monolistAddToFront(&val1);
    node = monoListConstFront(l);
    TEST_ASSERT_PTR_EQUAL(front, node);
    monoListFree(l);
}

void test_monoListBack_one() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monolistAddToFront(&val1);
    node = monoListBack(l);
    TEST_ASSERT_PTR_EQUAL(front, node);
    monoListFree(l);
}

void test_monoListConstBack_one() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monolistAddToFront(&val1);
    node = monoListConstBack(l);
    TEST_ASSERT_PTR_EQUAL(front, node);
    monoListFree(l);
}

void test_monoListSize_many() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    MonoListNode* node = NULL;
    for (int i = 0; i < 5; ++i) {
        node = monoListAddToFront(l, &i);
    }
    TEST_ASSERT_EQUAL_INT(monoListSize(l), 4);
    monoListFree(l);
}

void test_monoListItemSize_all() { // 1
    MonoList* l = monoListAlloc(sizeof(int));
    TEST_ASSERT_EQUAL_INT(monoListItemSize(l), sizeof(int));
    monoListFree(l);
}

void test_monoListNodeData() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monolistAddToFront(&val1);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val1);
    monoListFree(l);
}

void test_monoListConstNodeData() {//1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monolistAddToFront(&val1);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val1);
    monoListFree(l);
}

void test_monoListAddToFront_one() {// 0
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(l, &val1);
    TEST_ASSERT_NOT_NULL(front);
    int * data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val1);
    monoListFree(l);
}

void test_monoListAddToBack_one() {// 0
    MonoList* l = monoListAlloc(sizeof(int));
    int val2 = 2;
    MonoListNode* back = monoListAddToBack(l, &val2);
    TEST_ASSERT_NOT_NULL(back);
    int * data = monoListNodeData(back);
    TEST_ASSERT_EQUAL_INT(*data, val2);
    monoListFree(l);
}

void test_monoListNodeNext_many() {// 1
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    int i = 8;
    MonoListNode* back = monoListAddToFront(list, &i);
    int u = 9;
    front = monoListAddToFront(list, &u);
    monoListNode* next = monoListNodeNext(front);
    TEST_ASSERT_PTR_EQUAL(next, back);
    monoListFree(list);
}

void test_monoListPopFront_one() { // 1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(l, &val1);
    front = monoListPopFront(l);
    TEST_ASSERT_NULL(front);
    monoListFree(l);
}

void test_monoListDeleteNext_many() {// 2
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    for (int i = 1; i < 5; i++)
        front = monoListAddToFront(list, &i);
    MonoListNode* next = monoListNodeNext(front);
    MonoListNode* new_next = monoListNodeNext(next);
    MonoListNode* temp = monoListDeleteNext(front);
    TEST_ASSERT_PTR_EQUAL(temp, new_next);
    TEST_ASSERT_PTR_EQUAL(temp, monoListNodeNext(front));
    monoListFree(list);
}


void test_monoListRemoveNext_many() {// 2
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    for (int i = 1; i < 4; i++)
        front = monoListAddToFront(list, &i);
    MonoListNode* next = monoListNodeNext(front);
    MonoListNode* next_next = monoListNodeNext(next);
    MonoListNode* temp = monoListRemoveNext(list, front);
    TEST_ASSERT_PTR_EQUAL(next, temp);
    TEST_ASSERT_PTR_EQUAL(next_next, monoListNodeNext(front));
    monoListFree(list);
}

void test_monoListPopBack_two() {// 2
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    int val2 = 2;
    MonoListNode* back = monoListAddToBack(l, &val2);
    MonoListNode* front = monoListAddToFront(l, &val1);
    MonoListNode* back = monoListPopBack(l);
    TEST_ASSERT_PTR_EQUAL(back, front);
    monoListFree(l);
}


void test_monoListMoveNextToFront_two() {// 3
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    int val2 = 2;
    MonoListNode* front = monoListAddToFront(l, &val1);
    MonoListNode* back = monoListAddToBack(l, &val2);
    front = monoListMoveNextToFront(l, front);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val2);
    front = monoListFront(l);
    TEST_ASSERT_PTR_EQUAL(back, front);
    monoListFree(l);
}

void test_monoListMoveNextToBack_many() { // 4
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    int val2 = 2;
    int val3 = 3;
    MonoListNode* front = monoListAddToFront(l, &val1);
    MonoListNode* back = monoListAddToBack(l, &val2);
    MonoListNode* comp1 = back;
    MonoListNode* back = monoListAddToBack(l, &val3);
    MonoListNode* comp = back;
    back = monoListMoveNextToBack(l, front);
    front = monoListFront(l); 
    back = monoListBack(l);
    int* data = monoListNodeData(back);
    TEST_ASSERT_EQUAL_INT(*data, val2)
    TEST_ASSERT_PTR_NOT_EQUAL(comp, front);
    TEST_ASSERT_PTR_NOT_EQUAL(comp, back);
    TEST_ASSERT_PTR_EQUAL(comp1, back);
    monoListFree(l);
}

void test_monoListPrepend_many() {// 4
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* back;
    for (int i = 1; i < 4; i++)
        back = monoListAddToBack(list, &i);
    MonoListNode* temp = monoListRemoveNext(list, front);
    MonListNode* new_front = monoListPrepend(temp);
    TEST_ASSERT_PTR_EQUAL(new_front, temp);
    MonoListNode* front = monoListFront(list);
    TEST_ASSERT_PTR_EQUAL(front, temp);
    monoListFree(list);
}

void test_monoListAppend_many() {// 4
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    for (int i = 1; i < 4; i++)
        front = monoListAddToFront(list, &i);
    MonoListNode* temp = monoListRemoveNext(list, front);
    MonListNode* new_back = monoListAppend(temp);
    TEST_ASSERT_PTR_EQUAL(new_back, temp);
    MonoListNode* back = monoListBack(list);
    TEST_ASSERT_PTR_EQUAL(back, temp);
    monoListFree(list);
}