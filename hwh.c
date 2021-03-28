#include "ChainHashTable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

uint64_t stringHash(char const* str) {
    assert(str);

    uint64_t hash = 0;
    while(*str) {
        hash = hash * 31 + *(str++);
    }

    return hash;
}

bool stringCompare(char const* const left, char const* const right) {
    assert(left && right);

    return strcmp(left, right) == 0;
}

uint64_t stringHashForCHT(void const* const data) {
    assert(data);

    char const* const* str_p = data;

    return stringHash(*str_p);
}

bool stringCompareForCHT(void const* const left, void const* const right) {
    assert(left && right);

    char const* const* const left_p = left;
    char const* const* const right_p = right;

    return stringCompare(*left_p, *right_p);
}

char* readText() {
    size_t buffer_len = 0;
    if (scanf("%zu ", &buffer_len) != 1) {
        return NULL;
    }
    char* const buffer = calloc(buffer_len + 1, sizeof(*buffer));
    if (!buffer) {
        return NULL;
    }

    for (size_t i = 0; i < buffer_len; i++) {
        int c = getchar();
        if (c == EOF) {
            free(buffer);
            return NULL;
        }
        buffer[i] = c;
    }

    return buffer;
}

bool insertText(ChainHashTable* const table, char* const text) {
    assert(table && text);

    const char delim[] = " ";
    char const* token = strtok(text, delim);
    while (token) {
        size_t* const count_p = chainHashTableFind(table, &token);
        if (count_p) {
            (*count_p)++;
        } else {
            const size_t count = 1;
            if (!chainHashTableInsert(table, &token, &count)) {
                return false;
            }
        }

        token = strtok(NULL, delim);
    }

    return true;
}

void printWordCount(ChainHashTable const* const table, char* const words) {
    assert(table && words);

    const char delim[] = " ";
    char const* token = strtok(words, delim);
    while (token) {
        size_t const* const count_p = chainHashTableFind(table, &token);
        const size_t count = (count_p) ? (*count_p) : (0);
        printf("%zu ", count);

        token = strtok(NULL, delim);
    }

    puts("");
}

int main() {
    scanf("%*zu");
    char* const text = readText();
    char* const words = readText();

    ChainHashTable* const table = chainHashTableInit(sizeof(char const*), alignof(char const*),
                                                     sizeof(size_t), alignof(size_t),
                                                     stringHashForCHT, stringCompareForCHT);
    if (!text || !words || !table || !insertText(table, text)) {
        free(text);
        free(words);
        chainHashTableFree(table);
        return -1;
    }

    printWordCount(table, words);

    free(text);
    free(words);
    chainHashTableFree(table);

    return 0;
}
