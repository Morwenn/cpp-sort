#include <stdlib.h>
#include <stdio.h>

void swap(int *a, int *b) 
{
    int c = *a;
    *a = *b;
    *b = c;
}

int partition(int numbers[], int first, int last)
{
    int pivot = numbers[last];
    int i = first - 1;
    int j;

    for(j = first; j < last; j ++)
        // If the current element is smaller than or equal to the pivot, put the current
        // element at the beginning of the array
        if(numbers[j] <= pivot)
        {
            i ++;
            swap(&numbers[i], &numbers[j]);
        }

    swap(&numbers[i + 1], &numbers[last]);
    return (i + 1);
}

void quicksort(int numbers[], int first, int last)
{
    // while the array is not sorted
    if(first < last)
    {
        // we partition the given array between the given bounds
        int i = partition(numbers, first, last);

        // sort the first half and the second half
        quicksort(numbers, first, i - 1);
        quicksort(numbers, i + 1, last);
    }
}

int main(void)
{
    int *numbers = NULL;
    int n = 0, i = 0;

    // read the input
    printf("Number of elements:\n");
    scanf("%d", &n);

    // allocate memory
    numbers = (int*)calloc(n, sizeof(int));
    for(i = 0; i < n; i ++)
        scanf("%d", &numbers[i]);

    // sort the array
    quicksort(numbers, 0, n - 1);

    // print the array
    for(i = 0; i < n;i ++)
        printf("%d ", numbers[i]);
    printf("\n");

    return 0;
}
