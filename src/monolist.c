#include <monolist.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


struct MonoListNode_t {
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

MonoListNode* monoListAddToFront(MonoList* ml, void const* value) {
    assert(ml && "the list is not defined");
    MonoListNode* newstart = calloc(1, sizeof(MonoListNode) + ml->key_size);
    memcpy(newstart->data, value, ml->key_size);
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

MonoListNode* monoListAddToBack(MonoList* ml, void const* value) {
    assert(ml && "the list is not defined");
    MonoListNode* newend = calloc(1,sizeof(MonoListNode) + ml->key_size);
    memcpy(newend->data, value, ml->key_size);
    ml->length++;
    newend->next = NULL;
    if (ml->end = NULL) {
        ml->start = newend;
        ml->end = newend;
        return ml->end;
    }
    ml->end->next = newend;
    ml->end = newend;
    return ml->end;
}

MonoListNode* monoListPopBack(MonoList* ml) { //not sure if the old end should be deleted here
    assert(ml && "the list is not defined");
    assert(ml->end && "the list has no end...");
    ml->length--;
    if (ml->start == ml->end) {
        ml->start = NULL;
        ml->end = NULL;
        return ml-start;
    }
    MonoListNode* newend = ml->start;
    while (newend->next != ml->end) {
        newend = newend->next;
    }
    MonoListNode* outcast = ml->end;
    ml->end = newend;
    free(outcast);
    newend->next = NULL;
    return ml->end;
}

MonoListNode* monoListPopFront(MonoList* ml) {
    assert(ml && "the list is not defined");
    assert(ml->start && "there is nothing to cut out");
    ml->length--;
    if (ml->start == ml->end) {
        ml->start = NULL;
        ml->end = NULL;
        return ml->start;
    }
    MonoListNode* outcast = ml->start;
    ml->start = ml->start->next;
    free(outcast);
    return ml->start;
}

MonoListNode* monoListMoveNextToFront(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    assert(prenode && "the previous node is not defined");
    ml->length++;
    MonoListNode* node = monoListRemove(ml, prenode);
    node->next = ml->start;
    ml->start = node;
    return ml->start;
}

MonoListNode* monoListMoveNextToBack(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    assert(prenode && "the previous node is not defined");
    ml->length++;
    MonoListNode* node = monoListRemove(ml, prenode);
    ml->end->next = node;
    ml->end = node;
    return ml->end;
}

MonoListNode* monoListRemoveNext(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    MonoListNode* node = prenode->next;
    ml->length--;
    prenode->next = prenode->next->next;
    node->next = NULL;
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

MonoListNode* monoListDeleteNext(MonoList* ml, MonoListNode* prenode) {
    assert (prenode->next && "there is nothing to delete");
    MonoListNode* nextnode = prenode->next->next;
    free(prenode->next);
    prenode->next = nextnode;
    ml->length--;
    return nextnode;
}

MonoListNode* monoListAppend(MonoList* ml, MonoListNode* node) {
    assert(ml && "the list is not defined");
    assert(node && "the node is not defined");
    assert(!node && "you are trying to add a bunch of nodes");
    ml->length++;
    ml->end->next = node;
    ml->end = node;
    return ml->end;
}

MonoListNode* monoListPrepend(MonoList* ml, MonoList* node) {
    assert(ml && "the list is not defined");
    assert(node && "the node is not defined");
    ml->length++;
    node->next = ml->start;
    ml->start = node;
    return ml->start;
}





