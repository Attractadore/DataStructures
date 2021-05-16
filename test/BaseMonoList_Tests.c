// MonoList tests
#include <stdlib.h>
#include "BaseMonoList.c"

//typedef enum type TypeOfValue, for tests
typedef enum type {
    TYPE_OF_VALUE_INT,
    TYPE_OF_VALUE_UINT,
    TYPE_OF_VALUE_LINT,
    TYPE_OF_VALUE_FLOAT,
    TYPE_OF_VALUE_STRING,
} TypeOfValue;

// monoListShowList - prints list, send as "type" const TYPE_OF_VALUE_"your type"
void monoListShowList(MonoList* list, TypeOfValue type);

void monoListShowList(MonoList* list, TypeOfValue type)
{
    if (list->start == NULL)
        printf("List is empty!\n");
    else {
        MonoListNode* tmp = list->start;
        while (tmp != NULL) {
            switch (type) {
            case TYPE_OF_VALUE_INT: {
                printf("%d\n", *(int*)(tmp->data));
                break;
            }
            case TYPE_OF_VALUE_UINT: {
                printf("%u\n", *(unsigned int*)(tmp->data));
                break;
            }
            case TYPE_OF_VALUE_LINT: {
                printf("%lu\n", *(unsigned long*)(tmp->data));
                break;
            }
            case TYPE_OF_VALUE_FLOAT: {
                printf("%g\n", *(float*)(tmp->data));
                break;
            }
            case TYPE_OF_VALUE_STRING: {
                printf("%s\n", (char*)(tmp->data));
                break;
            }
            default:
                printf("Error type\n");
                break;
            }
            tmp = tmp->next;
        }

        printf("\n");
    }
}


int main() {
    char* b = "qwertyuiopasdfghjkl";
    MonoList* list = monoListAlloc(sizeof(b));
    char* tmp = calloc(sizeof(b), sizeof(char));
    for (int i = 2; i < 8; i++) {
        int j = i;
        while (j > 0) {
            tmp[i - j] = b[j];
            j--;
        }
        tmp[i] = '\0';
        monoListAddToFront(list, (const void*) (tmp));
    }
    monoListShowList(list, TYPE_OF_VALUE_STRING);
    MonoListNode* node = NULL;
    for (int i = 18; i > 15; i--) {
        monoListPopBack(list);
        int j = i;
        while (j > 15) {
            tmp[i - j] = b[j];
            j--;
        }
        tmp[i - j] = '\0';
        if(!node)
            node = monoListAddToFront(list, (const void*) (tmp));
        else
            monoListAddToFront(list, (const void*)(tmp));
    }
    monoListShowList(list, TYPE_OF_VALUE_STRING);

    for (int i = 0; i < 3; i++)
        monoListMoveNextToFront(list, node);
    monoListShowList(list, TYPE_OF_VALUE_STRING);

    monoListFree(list);
    monoListShowList(list, TYPE_OF_VALUE_STRING);

    return 0;
}

