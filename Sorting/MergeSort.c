#include <stdio.h>
#define max 10                                              //length of unsorted array

int a[10] = { 10, 14, 19, 26, 27, 31, 33, 35, 42, 44 };     //the array to be sorted
int b[10];                                                  //temporary array

//function that merges two arrays into one
void merging(int low, int mid, int high) {
   
   int l1, l2, i;

   //l1 and l2 point to one element each in two different arrays
   //Initially, both are set to the starting element of resp array
   //Consider an array a[] divided into two parts 'left' and 'right' by the middle element 'mid'
   //Now, l1 and l2 are the first elements of 'left' and 'right' respectively
   //Since we divide the unsorted array to the smallest parts before starting to merge them back, it is guaranteed that left and right are themselves sorted
   //Therefore, we loop through both left and right and at each iteration i, we set b[i] to the next smallest element from left and right.
   for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++) {
      if(a[l1] <= a[l2])
         b[i] = a[l1++];
      else
         b[i] = a[l2++];
   }
   
   //if 'right' gets exhausted while 'left' still has some elements, we add them to b[] at the end
   while(l1 <= mid)    
      b[i++] = a[l1++];

   //if 'left' gets exhausted while 'right' still has some elements, we add them to b[] at the end
   while(l2 <= high)   
      b[i++] = a[l2++];

   //Now, we replace the original unsorted array a[] with the sorted array b[]
   for(i = low; i <= high; i++)
      a[i] = b[i];
}

//function that sorts the unsorted array
void sort(int low, int high) {
   int mid;
   
   if(low < high) {                             //If there's more than one element
      mid = (low + high) / 2;                   //we divide the array into two
      sort(low, mid);                           //we sort the left part individually
      sort(mid+1, high);                        //and the right part individually
      merging(low, mid, high);                  //and then we merge both of them together
   } else {                                     //However, if there's just one element
      return;                                   //there's not much to do
   }   
}

int main() { 
   int i;

   printf("List before sorting\n");
   
   for(i = 0; i <= max; i++)
      printf("%d ", a[i]);

   sort(0, max);

   printf("\nList after sorting\n");
   
   for(i = 0; i <= max; i++)
      printf("%d ", a[i]);
}
