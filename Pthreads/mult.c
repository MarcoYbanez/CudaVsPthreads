#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1024
#define DOMAIN 100
#define THREADS 4

  int* A_mat;
  int* B_mat;
  int* C_mat;

void print_Mat(int** M);


void* multiply( void* arg ){
  
  int work = N/THREADS; //count iterations

  int* thread_id = (int *)arg;
  

  int i,j,k;
  for(i = (*thread_id)*work; i < (*thread_id)*work + work; ++i){
    for(j = 0; j < N; ++j){
      C_mat[i*N + j] = 0;
      for(k = 0; k < N; ++k){
        C_mat[i*N + j] += A_mat[i*N + k] * B_mat[k*N + j];
      }
    }
  }

  thread_id = NULL;
  free(arg);
  arg = NULL;


}

int main(){

  //allocate and populate arrays
  int i, j, k;
  time_t t;
  struct timespec start, finish;
  double fin_time;
  srand((unsigned) time(&t));
  
  A_mat = (int*)malloc(N * N * sizeof(int));
  B_mat = (int*)malloc(N * N * sizeof(int));
  C_mat = (int*)malloc(N * N * sizeof(int));

  for(i = 0; i  < N; ++i){
    for(j=0; j < N; ++j){
      A_mat[i*N + j] = rand() % DOMAIN +1;
      B_mat[i*N + j] = rand() % DOMAIN +1;
    }
  }

  //print_Mat(A_mat);
  //print_Mat(B_mat);
  pthread_t *threads;
  threads = (pthread_t*)malloc(THREADS * sizeof(pthread_t) );

  int* data;
  int count = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i = 0; i < THREADS; ++i){

    int* thread_num = (int*)malloc(sizeof(int));
    *thread_num = i;
    pthread_create(&threads[i], NULL, multiply, (void*)thread_num);
  }

  for(i = 0; i < THREADS; ++i){
    pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  fin_time = (finish.tv_sec - start.tv_sec);
  fin_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


  //printf("\n\n");
  //print_Mat(A_mat);
  //printf("\n\n");
  //print_Mat(B_mat);
  //printf("\n\n");
  //print_Mat(C_mat);

  printf("Time to complete: %f\n", fin_time);

  // Multiply arrays
  free(A_mat);
  free(B_mat);
  free(C_mat);
  
  free(threads);
  return 0;
}


void print_Mat(int** M){

  int i, j;
  for(i = 0; i < N; ++i){
    for(j = 0; j < N; ++j){
      printf("%d ", M[i][j]);
    }
    printf("\n");
  }
}

