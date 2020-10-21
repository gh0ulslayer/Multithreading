INPUT : n (length of array ) , followed by n integers.



Allocated shared memory : 
A shared memory region(array to be sorted) is created for the multiprocesses to share data using the function "sharemem".

Selection Sort : 
for n <= 5 , selection sort will be run in  place of concurrent merge sort.

Normal merge sort : 
Function "normal_mergeSort" break the array into two halfs for sorting and merging back.

Concurrent MergeSort :
The function "mergeSort" forks into two child processes until they are recursively sorted and the parent process waits until the child processes are merged .

Conclusion :

The concurrent mergesort is slower than normal mergesort because of creating multiple processes.
