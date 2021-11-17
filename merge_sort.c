#include <stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <time.h>

#define MAX 100

typedef struct Info{
    int left;
    int right;
}Info;

int A[MAX];
int temp[MAX];
// Merge sort function
void *mergeSort(void* arg) {
    
    Info thread_info = *((Info*)arg);
    int left = thread_info.left;
    int right = thread_info.right;
    
    if (left<right) {
    
    int mid = (left + right) / 2;

    pthread_t thread1;
    Info d1 = {left, mid};
    
    pthread_t thread2;
    Info d2 = {mid+1, right};
    
    pthread_create(&thread1, NULL, &mergeSort, &d1);
    pthread_create(&thread2, NULL, &mergeSort, &d2);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
  
    //Two pointers for merging
    int start = left;       
    int end = mid + 1;        
    
    int k;
    for (k = left; k <= right; k++) {
        if (start == mid + 1) {      
            temp[k] = A[end];
            end++;
        } else if (end == right + 1) {     
            temp[k] = A[start];
            start++;
        } else if (A[start] < A[end]) {
            temp[k] = A[start];
            start++;
        } else {
            temp[k] = A[end];
            end++;
        }
    }

    //copy merged array to our original Array A
    for (k = left; k <= right; k++) {
        A[k] = temp[k];
    }
    }
}

int n, i;
void read_data(const char *filename){
    FILE* file;
    file = fopen(filename, "r");
    fscanf(file, "%d", &n);
    for(i=0; i<n; i++)
        fscanf(file, "%d", &A[i]);
    fclose(file);
}

int main() {
    read_data("input.txt");
    Info thread_info = {0, n-1};
    
    clock_t timer;
    timer = clock();
    
    pthread_t t;
    pthread_create(&t, NULL, &mergeSort, &thread_info);
    pthread_join(t, NULL);
 
    timer = clock() - timer;
    double time_taken = ((double)timer)/CLOCKS_PER_SEC; // in seconds
  
    printf("Merge sort took %f seconds equal %f milliseconds to execute \n", 
    time_taken, time_taken*1000);

  printf("Array after sorting:\n");
  for (i = 0; i < n; i++)
     printf("%d\n", A[i]);
 
  return 0;
}
