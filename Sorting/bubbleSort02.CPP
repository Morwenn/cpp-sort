#include<iostream>
#include<conio>

int bubbleSort(int ar[], int n)
{
	int i, j, temp, pass;
	for(i=0;i<n-1;i++)
   {
   	pass=0;
   	for(j=0;j<n-i-1;j++)
      {
      	if(ar[j]>ar[j+1])
         {
         	temp=ar[j];
            ar[j]=ar[j+1];
            ar[j+1]=temp;
            pass=1;
         }
         if(pass==0)
         	break;
      }
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

   bubbleSort(ar,n);
   disp(ar,n);
   getch();
}