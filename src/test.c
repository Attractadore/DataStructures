#include "MonoList.c"

void test_monoListMoveToFrontOneElement() {
    l = List();
    l.append();
    node = l.back();
    l.moveToFront();
    TEST_ASSERT_PTR_EQUAL(l.start, node);
    TEST_ASSERT_PTR_EQUAL(l.end, node);
    TEST_ASSERT_PTR_EQUAL(node->next, NULL);
}

void test_monoListAppend() {
    MonoList* l = monoListAlloc(sizeof(int));

}