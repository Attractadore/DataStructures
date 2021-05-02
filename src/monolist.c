#include <monolist.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


struct mononode_t {
    MonoListNode* next;
    unsigned char data[];
};

struct MonoList_T {
    MonoListNode* start;
    MonoListNode* end;
    size_t key_size;
    size_t length;
};

void monoListFree(MonoList* ml) {
    assert(ml && "the list is already empty");
    MonoListNode* cur = ml->start;
    while (cur != ml->end) {
        cur = cur->next;
        free(ml->start);
        ml->start = cur;
    }

    free(ml->start);
    free(ml);
}

MonoListNode* monoListAddFront(MonoList* ml, void const* value) {
    assert(ml && "the list is not defined");
    MonoListNode* newstart = calloc(1, sizeof(MonoList)ml->key_size);
    memcpy(list->start->data, value, list->key_size);
    ml->length++;
    if (ml->start == NULL) {
        ml->start = newstart;
        ml->end = ml->start;
        return ml->start;
    }
    newstart->next = ml->start;
    ml->start = newstart;
    return ml->start;
}

MonoListNode* monoListPopBack(MonoList* ml) { //not sure if the old end should be deleted here
    assert(ml && "the list is not defined");
    assert(ml->end && "the list has no end...");
    ml->length--;
    if (ml->start == ml->end) {
        ml->start = NULL;
        ml->end = NULL;
    }
    MonoListNode* newend = ml->start;
    while (newend->next != ml->end) {
        newend = newend->next;
    }
    ml->end = newend;
    newend->next = NULL;
    return ml->end;
}

MonoListNode* monoListMoveToFront(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    assert(prenode && "the prvious node is not defined");
    ml->length++;
    MonoListNode* node = monoListRemove(ml, prenode);
    node->next = ml->start;
    ml->start = node;
    return ml->start;
}

MonoListNode* monoListRemove(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    MonoListNode* node = prenode->next;
    ml->length--;
    prenode->next = prenode->next->next;
    return node;
}

MonoList* monoListAlloc(size_t key_size) {
    MonoList* newlist = calloc(1, sizeof(MonoList));
    newlist->start = NULL;
    newlist->end = NULL;
    newlist->key_size = key_size;
    newlist->length = 0;
    return newlist;
}


