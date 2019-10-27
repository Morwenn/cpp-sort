#include <stdlib.h>
#include <stdio.h>

// sort an array of int
void countingSort
    (int *vector,       // vector
     int len,           // vector size
     int maxElement)    // the biggest element of the vector
{
    int *count, i, j;
    count = (int*) malloc(sizeof(int) * (maxElement + 1));

    if (count == NULL) { 
        return;
    }

    for (i = 0; i < len; i++) {
        count[vector[i]]++;
    }

    for (j = 0, i = 0; i < len; j++) {
        while (count[j]-- > 0) {
            vector[i++] = j;
        }
    }
    free(count);
}

int main() {
    int vector[] = {20,12,7,2,10,35}, len = 6, max = 35, i;
    countingSort(vector, len, max);
    for (i = 0; i < len; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
    return 0;
}