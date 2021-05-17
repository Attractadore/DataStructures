#include "BaseMonoList.c"

#include "unity.h"

void test_monoListAlloc() { // 0
    MonoList* list = monoListAlloc(sizeof(int));
    TEST_ASSERT_NOT_NULL(list);
    monoListFree(list);
}
/*
void test_monoListFree() { // 0
    MonoList* list = monoListAlloc(sizeof(int));
    monoListFree(list);
    TEST_ASSERT_EMPTY(list);
}
*/

void test_monoListFront_one() {//1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    MonoListNode* node = monoListFront(list);
    TEST_ASSERT_EQUAL_PTR(front, node);
    monoListFree(list);
}


void test_monoListConstFront_one() {//1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    const MonoListNode* node = monoListConstFront(list);
    TEST_ASSERT_EQUAL_PTR(front, node);
    monoListFree(list);
}


void test_monoListBack_one() {//1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    MonoListNode* node = monoListBack(list);
    TEST_ASSERT_EQUAL_PTR(front, node);
    monoListFree(list);
}


void test_monoListConstBack_one() {//1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    const MonoListNode* node = monoListConstBack(list);
    TEST_ASSERT_EQUAL_PTR(front, node);
    monoListFree(list);
}


void test_monoListSize_many() {//1      ....
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* node;
    for (int i = 0; i < 5; i++) {
        node = monoListAddToFront(list, &i);
    }
    int size = monoListSize(list);
    TEST_ASSERT_EQUAL_INT(size, 5);
    monoListFree(list);
}


void test_monoListItemSize_all() { // 1
    MonoList* list = monoListAlloc(sizeof(int));
    int item_size = monoListItemSize(list);
    TEST_ASSERT_EQUAL_INT(item_size, sizeof(int));
    monoListFree(list);
}

/*
void test_monoListNodeData() {//1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val1);
    monoListFree(list);
}
*/
/*
void test_monoListConstNodeData() {//1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val1);
    monoListFree(list);
}
*/
/*
void test_monoListAddToFront_one() {// 0
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    TEST_ASSERT_NOT_NULL(front);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val1);
    monoListFree(list);
}
*/
/*
void test_monoListAddToBack_one() {// 0
    MonoList* list = monoListAlloc(sizeof(int));
    int val2 = 2;
    MonoListNode* back = monoListAddToBack(list, &val2);
    TEST_ASSERT_NOT_NULL(back);
    int* data = monoListNodeData(back);
    TEST_ASSERT_EQUAL_INT(*data, val2);
    monoListFree(list);
}
*/
/*
void test_monoListNodeNext_many() {// 1
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    int i = 8;
    MonoListNode* back = monoListAddToFront(list, &i);
    int u = 9;
    front = monoListAddToFront(list, &u);
    MonoListNode* next = monoListNodeNext(front);
    TEST_ASSERT_EQUAL_PTR(next, back);
    monoListFree(list);
}
*/
/*
void test_monoListPopFront_one() { // 1
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(list, &val1);
    front = monoListPopFront(list);
    TEST_ASSERT_NULL(front);
    monoListFree(list);
}
*/
/*
void test_monoListDeleteNext_many() {// 2
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    for (int i = 1; i < 5; i++)
        front = monoListAddToFront(list, &i);
    MonoListNode* next = monoListNodeNext(front);
    MonoListNode* new_next = monoListNodeNext(next);
    MonoListNode* temp = monoListDeleteNext(list, front);
    TEST_ASSERT_EQUAL_PTR(temp, new_next);
    TEST_ASSERT_EQUAL_PTR(temp, monoListNodeNext(front));
    monoListFree(list);
}
*/
/*
void test_monoListRemoveNext_many() {// 2
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    for (int i = 1; i < 4; i++)
        front = monoListAddToFront(list, &i);
    MonoListNode* next = monoListNodeNext(front);
    MonoListNode* next_next = monoListNodeNext(next);
    MonoListNode* temp = monoListRemoveNext(list, front);
    TEST_ASSERT_EQUAL_PTR(next, temp);
    TEST_ASSERT_EQUAL_PTR(next_next, monoListNodeNext(front));
    monoListFree(list);
}
*/
/*
void test_monoListPopBack_two() {// 2
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    int val2 = 2;
    MonoListNode* back = monoListAddToBack(list, &val2);
    MonoListNode* front = monoListAddToFront(list, &val1);
    back = monoListPopBack(list);
    TEST_ASSERT_EQUAL_PTR(back, front);
    monoListFree(list);
}
*/
/*

void test_monoListMoveNextToFront_two() {// 3
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    int val2 = 2;
    MonoListNode* front = monoListAddToFront(list, &val1);
    MonoListNode* back = monoListAddToBack(list, &val2);
    front = monoListMoveNextToFront(list, front);
    int* data = monoListNodeData(front);
    TEST_ASSERT_EQUAL_INT(*data, val2);
    front = monoListFront(list);
    TEST_ASSERT_EQUAL_PTR(back, front);
    monoListFree(list);
}
*/
/*
void test_monoListMoveNextToBack_many() { // 4
    MonoList* list = monoListAlloc(sizeof(int));
    int val1 = 1;
    int val2 = 2;
    int val3 = 3;
    MonoListNode* front = monoListAddToFront(list, &val1);
    MonoListNode* back = monoListAddToBack(list, &val2);
    MonoListNode* comp1 = back;
    back = monoListAddToBack(list, &val3);
    MonoListNode* comp = back;
    back = monoListMoveNextToBack(list, front);
    front = monoListFront(list);
    back = monoListBack(list);
    int* data = monoListNodeData(back);
    TEST_ASSERT_EQUAL_INT(*data, val2);
    // TEST_ASSERT_PTR_NOT_EQUAL(comp, front);
    // TEST_ASSERT_PTR_NOT_EQUAL(comp, back);
    TEST_ASSERT_EQUAL_PTR(comp1, back);
    monoListFree(list);
}
*/
/*
void test_monoListPrepend_many() {// 4
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* back;
    for (int i = 1; i < 4; i++)
        back = monoListAddToBack(list, &i);
    MonoListNode* front = monoListFront(list);
    MonoListNode* temp = monoListRemoveNext(list, front);
    MonoListNode* new_front = monoListPrepend(list, temp);
    TEST_ASSERT_EQUAL_PTR(new_front, temp);
    front = monoListFront(list);
    TEST_ASSERT_EQUAL_PTR(front, temp);
    monoListFree(list);
}
*/
/*
void test_monoListAppend_many() {// 4
    MonoList* list = monoListAlloc(sizeof(int));
    MonoListNode* front;
    for (int i = 1; i < 4; i++)
        front = monoListAddToFront(list, &i);
    MonoListNode* temp = monoListRemoveNext(list, front);
    MonoListNode* new_back = monoListAppend(list, temp);
    TEST_ASSERT_EQUAL_PTR(new_back, temp);
    MonoListNode* back = monoListBack(list);
    TEST_ASSERT_EQUAL_PTR(back, temp);
    monoListFree(list);
    }
    */