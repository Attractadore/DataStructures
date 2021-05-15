#include "BaseMonoList.h"
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
    if (ml == NULL)
        return;
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
    if (newstart == NULL)
        return NULL;
    memcpy(newstart->data, value, ml->key_size);
    return monoListPrepend(ml, newstart);
}

MonoListNode* monoListAddToBack(MonoList* ml, void const* value) {
    assert(ml && "the list is not defined");
    MonoListNode* newend = calloc(1,sizeof(MonoListNode) + ml->key_size);
    if (newend == NULL)
        return NULL;
    memcpy(newend->data, value, ml->key_size);
    return monoListAppend(ml, newend);
}

MonoListNode* monoListPopBack(MonoList* ml) { //not sure if the old end should be deleted here
    assert(ml && "the list is not defined");
    assert(ml->end && "the list has no end...");
    ml->length--;
    if (ml->start == ml->end) {
        free(ml->start);
        ml->start = NULL;
        ml->end = NULL;
        return NULL;
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
        free(ml->start);
        ml->start = NULL;
        ml->end = NULL;
        return NULL;
    }
    MonoListNode* outcast = ml->start;
    ml->start = ml->start->next;
    free(outcast);
    return ml->start;
}

MonoListNode* monoListMoveNextToFront(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    assert(prenode && "the previous node is not defined");
    return monoListPrepend(ml, monoListRemoveNext(ml, prenode));
}

MonoListNode* monoListMoveNextToBack(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    assert(prenode && "the previous node is not defined");
    return monoListAppend(ml, monoListRemoveNext(ml, prenode));
}

MonoListNode* monoListRemoveNext(MonoList* ml, MonoListNode* prenode) {
    assert(ml && "the list is not defined");
    if (prenode->next == NULL)
        return NULL;
    MonoListNode* node = prenode->next;
    ml->length--;
    prenode->next = prenode->next->next;
    node->next = NULL;
    return node;
}

MonoList* monoListAlloc(size_t key_size) {
    MonoList* newlist = calloc(1, sizeof(MonoList));
    if (newlist == NULL)
        return NULL;
    newlist->key_size = key_size;
    return newlist;
}

MonoListNode* monoListDeleteNext(MonoList* ml, MonoListNode* prenode) {
    assert (prenode->next && "there is nothing to delete");
    free(monoListRemoveNext(ml, prenode));
    return prenode->next;
}

MonoListNode* monoListAppend(MonoList* ml, MonoListNode* node) {
    assert(ml && "the list is not defined");
    assert(node && "the node is not defined");
    assert(!(node->next) && "you are trying to add a bunch of nodes");
    ml->length++;
    if (ml->end == NULL) {
        ml->start = node;
        ml->end = node;
        return ml->end;
    }
    ml->end->next = node;
    ml->end = node;
    return ml->end;
}

MonoListNode* monoListPrepend(MonoList* ml, MonoListNode* node) {
    assert(ml && "the list is not defined");
    assert(node && "the node is not defined");
    ml->length++;
    node->next = ml->start;
    ml->start = node;
    if (ml->end == NULL)
        ml->end = node;
    return ml->start;
}

MonoListNode* monoListFront(MonoList* ml) {
    assert(ml && "the list is not defined");
    return ml->start;
}
MonoListNode* monoListBack(MonoList* ml) {
    assert(ml && "the list is not defined");
    return ml->end;
}
void* monoListNodeData(MonoListNode* node) {
    assert(node && "the node is not defined");
    return node->data;
}
MonoListNode const* monoListConstFront(MonoList const* ml) {
    assert(ml && "the list is not defined");
    return ml->start;
}
MonoListNode const* monoListConstBack(MonoList const* ml) {
    assert(ml && "the list is not defined");
    return ml->end;
}
void const* monoListConstNodeData(MonoListNode const* node) {
    assert(node && "the node is not defined");
    return node->data;
}

size_t monoListItemSize(MonoList* ml) {
    return ml->key_size;
}

size_t monoListSize(MonoList* ml) {
    return ml->length;
}

MonoListNode* monoListNodeNext(MonoListNode* node) {
    return node->next;
}




