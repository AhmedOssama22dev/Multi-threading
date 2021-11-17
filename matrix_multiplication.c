#include <stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <time.h>

typedef struct Info{
    int a[10][10];
    int b[10][10];
}Info;

// global variables declaration
int a[10][10],b[10][10],mul[10][10],mul_row[10][10],r,c,i,j,k;

// function to print the result of multiplication  
void print_matrix(int matrix[10][10]){
printf("\nResulted matrix:\n");   
    for(i=0;i<r;i++){    
    for(j=0;j<c;j++){    
    printf("%d\t",matrix[i][j]);    
    }    
    printf("\n");    
    }    
}
//func to handle cell based threading
void* multiply(void *arg){
mul[i][j]+=a[i][k]*b[k][j];
}

//func to handle row base threading
void* multiply_row(void *arg){
    for(k=0;k<c;k++)    
    {    
    mul_row[i][j]+=a[i][k]*b[k][j];
    }  
}
//func to read from file input
int r_c[2];
void read_data(const char *filename){
    FILE* file;
    file = fopen(filename, "r");
    for(int l=0;l<2;l++)
    fscanf(file, "%d", &r_c[l]);
    r = r_c[0];
    c = r_c[1];
    for(i=0;i<r;i++){
    for(j=0; j<c; j++){
        fscanf(file, "%d", &a[i][j]);
    }}
    
    for(int l=0;l<2;l++)
    fscanf(file, "%d", &r_c[l]);
    r = r_c[0];
    c = r_c[1];
    for(i=0;i<r;i++){
    for(j=0; j<c; j++){
        fscanf(file, "%d", &b[i][j]);
    }}

    fclose(file);
}

//row threaded
void row_threaded_mul(){
        //row threading
    clock_t timer_no_thread;
    timer_no_thread = clock();
     
    for(i=0;i<r;i++)    
    {    
    for(j=0;j<c;j++)    
    {    
    mul_row[i][j]=0;    
    
    pthread_t thread2;
    Info d2 = {a[i][k], b[k][i]}; 
    pthread_create(&thread2, NULL, &multiply_row, &d2);
    pthread_join(thread2, NULL);
    }    
    } 
       
    timer_no_thread = clock() - timer_no_thread;
    double time_taken1 = ((double)timer_no_thread)/CLOCKS_PER_SEC; // in seconds
    printf("\nRow threaded matrix:\n");
    print_matrix(mul_row);
    printf("Elapsed time: %f seconds = %f milliseconds\n", 
    time_taken1, time_taken1*1000);
    
}
//cell threaded
void cell_threaded(){
       clock_t timer;
    timer = clock();
    //each element in a thread
       
    for(i=0;i<r;i++)    
    {    
    for(j=0;j<c;j++)    
    {    
    mul[i][j]=0;    
    for(k=0;k<c;k++)    
    {    
        
    pthread_t thread1;
    Info d1 = {a[i][k], b[k][i]}; 
    pthread_create(&thread1, NULL, &multiply, &d1);
    pthread_join(thread1, NULL);

    }    
    }    
    }    
    timer = clock() - timer;
    double time_taken2 = ((double)timer)/CLOCKS_PER_SEC; // in seconds
    printf("\nCell threaded matrix:\n");
    print_matrix(mul);
    printf("Elapsed time: %f seconds = %f milliseconds\n", 
    time_taken2, time_taken2*1000);
    
}

    int main(){    
    read_data("input1.txt");

    row_threaded_mul();
    cell_threaded();

    return 0;  
    }  
