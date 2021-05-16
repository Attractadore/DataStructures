#include "monolist.c"
#include "unity.h"

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

void test_monoListPopFront_one() { // 1
    MonoList* l = monoListAlloc(sizeof(int));
    int val1 = 1;
    MonoListNode* front = monoListAddToFront(l, &val1);
    front = monoListPopFront(l);
    TEST_ASSERT_NULL(front);
    monoListFree(l);
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
    front = monoListMoveNextToFront(l, front); // ???
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
    front = monoListFront(l); // ???
    back = monoListBack(l);
    int* data = monoListNodeData(back);
    TEST_ASSERT_EQUAL_INT(*data, val2)
    TEST_ASSERT_PTR_NOT_EQUAL(comp, front);
    TEST_ASSERT_PTR_NOT_EQUAL(comp, back);
    TEST_ASSERT_PTR_EQUAL(comp1, back);
    monoListFree(l);
}



