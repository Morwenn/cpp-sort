#include<iostream>
#include<conio>

int selectionSort(int ar[], int n)
{
	int i, j, temp, min;
	for(i=0;i<n-1;i++)
   {
   	min=i;
      for(j=i+1;j<n;j++)
      {
      	if(ar[j]<ar[min])
         {
         	min=j;
         }
      }
      temp=ar[min];
      ar[min]=ar[i];
      ar[i]=temp;
   }
   return 0;
}

int disp(int ar[], int n)
{
	cout<<"\nSorted array is : ";
	for(int i=0;i<n;i++)
   {
   	cout<<ar[i]<<" ";
   }
   return 0;
}

int main(void)
{
	int n, ar[10],i;
	cout<<"Enter the number of elements\n";
   cin>>n;
   cout<<"\nEnter the array\n";
   for(i=0;i<n;i++)
   {
   	cin>>ar[i];
   }

   selectionSort(ar,n);
   disp(ar,n);
   getch();
}