#include <stdio.h>
#include <stdlib.h>
//implementation of insertion sort in C
//@Author Tiberiu Lepadatu
//function declarations
void insertion_sort(int *arr, int n);
void display (int *arr, int n); 


int main (void){
	int n, *arr, i;

	//reading the numbers that we compute
	printf("Please provide the length of the array: ");
	scanf("%d", &n);
	printf("\n");
	arr = malloc(sizeof(int)*n);
	printf("Please specify the numbers you want in the array: ");
	for (i = 0; i < n; i++){
		scanf("%d", &arr[i]);
	}
	printf("\n");

	insertion_sort(arr, n);
	display(arr, n);

	return 0;
}


void display (int *arr, int n){
	int i;

	printf ("The sorted number array is: \n");
	for (i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}

	printf("\n");
	
}
void insertion_sort (int *arr, int n){
	//you just iterate through the numbers and place each where it is
	//supposed to stay; note that the method is stable 
	int i, j, pos;	
	
	for (i = 1; i < n; i++){
		j = i-1;
		pos = arr[i];
		while (j >= 0 && arr[j] > pos){
			arr[j+1] = arr[j];
			j--;
		}
		arr[j+1] = pos;
	}
}
