# Python program for implementation of Bubble Sort
 
def bubbleSort(arr):
    n = len(arr)
 
    # Traverse through all array elements
    for i in range(n):
 
        # Last i elements are already in place
        for j in range(0, n-i-1):
 
            # traverse the array from 0 to n-i-1
            # Swap if the element found is greater
            # than the next element
            if arr[j] > arr[j+1] :
                arr[j], arr[j+1] = arr[j+1], arr[j]
 
# Driver code to test above
arr = []
n=input("Enter the length of array :")    # taking the size of array
for a in xrange(n):
    arr.append(input("Enter the element :"))    #adding element to the array 
bubbleSort(arr) #calling the function
print ("Sorted array is:")
for i in xrange(n):     #element wise printing the array
    print ("%d" %arr[i]),
