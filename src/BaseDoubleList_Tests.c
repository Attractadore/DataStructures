// DoubleList tests
#include "BaseDoubleList.h"
#include <stdlib.h>

int main() { 
	char* b = "qwertyuiopasdfghjkl";
	DoubleList* list = doubleListAlloc(sizeof(b));
	char* tmp = calloc(sizeof(b), sizeof(char));
	for (int i = 2; i < 8; i++) {
		int j = i;
		while (j > 0) {
			tmp[i - j] = b[j];
			j--;
		}
		tmp[i] = '\0';
		doubleListAddFront(list, (const void*)(tmp));
	}
	doubleListShowList(list, TYPE_OF_VALUE_STRING);

	for (int i = 18; i > 15; i--) {
		doubleListPopBack(list);
		int j = i;
		while (j > 15) {
			tmp[i - j] = b[j];
			j--;
		}
		tmp[i - j] = '\0';
		doubleListAddFront(list, (const void*)(tmp));
	}
	doubleListShowList(list, TYPE_OF_VALUE_STRING);

	for (int i = 0; i < 3; i++)
		doubleListMoveToFront(list, list->end);
	doubleListShowList(list, TYPE_OF_VALUE_STRING);
	
	doubleListFree(list);
	doubleListShowList(list, TYPE_OF_VALUE_STRING);

	return 0;
}