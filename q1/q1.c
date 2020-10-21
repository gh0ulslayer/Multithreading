#define _POSIX_C_SOURCE 199309L //required for clock
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>

void swap(int *xp, int *yp) ;
int * shareMem(size_t size);
void merge(int a[], int l1, int h1, int h2); 
void selectionSort(int arr[], int n);
void normal_mergeSort(int arr[], int l, int r) ;
void mergeSort(int a[], int l, int h) ;


void swap(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
int * shareMem(size_t size)
{
     key_t mem_key = IPC_PRIVATE;
     int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
     return (int*)shmat(shm_id, NULL, 0);
}

void selectionSort(int arr[], int n)  
{  
    int min_idx;  
  
    for (int i = 0; i < n-1; i++)  
    {  
        min_idx = i;  
        for (int j = i+1; j <= n-1; j++)  
        if ( arr[min_idx] > arr[j] )  
            min_idx = j;  
  
        swap(&arr[min_idx], &arr[i]);  
    }  
}   
void normal_mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l + (r - l) / 2; 
        normal_mergeSort(arr, l, m); 
        normal_mergeSort(arr, m + 1, r); 
  
        merge(arr, l, m, r); 
    } 
} 
void mergeSort(int a[], int l, int h) 
{ 
    int i, len=(h-l+1); 
  
    if (len<=5) 
    { 
        selectionSort(a+l, len); 
        return; 
    } 
  
    pid_t lpid;
    pid_t rpid; 
    lpid = fork(); 
    if (lpid == 0 ) 
    { 
        mergeSort(a,l,l+len/2-1); 
        _exit(0); 
    }
    else  if (lpid < 0) 
    { 
        perror("Left CP not created\n"); 
        _exit(-1); 
    } 
    else
    { 
        rpid = fork(); 
        if(rpid==0) 
        { 
            mergeSort(a,l+len/2,h); 
            _exit(0); 
        } 
        else if (rpid<0) 
        { 
            perror("Right CP not created\n"); 
            _exit(-1); 
        } 
    } 
  
    int status; 
    waitpid(lpid, &status, 0); 
    waitpid(rpid, &status, 0); 
    merge(a, l, l+len/2-1, h); 
} 

void merge(int a[], int l1, int h1, int h2) 
{ 
    int count=h2-l1+1; 
    int sorted[count]; 
    int i=l1, k=h1+1, m=0; 
    while (i<=h1 && k<=h2) 
    { 
        if (a[i]==a[k]) 
        { 
            sorted[m]=a[i]; 
            m++;i++;
            sorted[m]=a[k];
            m++;k++;

        } 
        else if (a[i]<a[k]) 
        {
            sorted[m]=a[i]; 
            m++;i++;
        }
        else if (a[k]<a[i]) 
        {
            sorted[m]=a[k]; 
            m++;k++;
        }
    } 
  
    while (i<=h1) 
    {
        sorted[m]=a[i]; 
        m++;i++;
    }
    while (k<=h2)
    { 
        sorted[m]=a[k]; 
            m++;k++;
    }
    
    int arr_count = l1; 
    for (i=0; i<count; i++,l1++) 
        a[l1] = sorted[i]; 
} 
  


void runSorts(long long int n){

     struct timespec ts;

     //getting shared memory
     int *arr = shareMem(sizeof(int)*(n+1));
     for(int i=0;i<=n-1;i++) 
        {
     //   scanf("%d", arr+i);
            arr[i]=rand()%1001;
    }
     int brr[n+1];
     for(int i=0;i<=n-1;i++) 
        brr[i] = arr[i];

     printf("Running concurrent_mergesort for n = %lld\n", n);
     clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
     long double st = ts.tv_nsec/(1e9)+ts.tv_sec;

     //multiprocess mergesort
     mergeSort(arr, 0, n-1);
     for(int i=0; i<n; i++){
          printf("%d ",arr[i]);
     }
     printf("\n");
     clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
     long double en = ts.tv_nsec/(1e9)+ts.tv_sec;
     printf("time = %Lf\n", en - st);
     long double t1 = en-st;

     

     printf("Running normal_mergesort for n = %lld\n", n);
     clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
     st = ts.tv_nsec/(1e9)+ts.tv_sec;

     // normal mergesort
     normal_mergeSort(brr, 0, n-1);
     for(int i=0; i<n; i++){
          printf("%d ",brr[i]);
     }
     printf("\n");
     clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
     en = ts.tv_nsec/(1e9)+ts.tv_sec;
     printf("time = %Lf\n", en - st);
     long double t3 = en - st;

     printf("normal_mergesort ran:\n\t[ %Lf ] times faster than concurrent_mergesort\n\t\n", t1/t3);
     shmdt(arr);
     return;
}

int main(){

     long long int n;
     scanf("%lld", &n);
     runSorts(n);
     return 0;
}
