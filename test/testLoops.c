#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    int k, n;
    scanf("%d %d", &k, &n);
    printf("%d\n", k * n);
    int* arr = calloc(k, sizeof(int));
    for (int i = 0; i < k; i++)
        arr[i] = rand() % 1000;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++)
            printf("%d\n", arr[j]);

}

