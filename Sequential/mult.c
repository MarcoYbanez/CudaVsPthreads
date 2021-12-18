#include <stdio.h>
#include <time.h>
#include <time.h>
#include <stdlib.h>

#define N 1024
#define DOMAIN 100

// Verify using
// Freivalds' algorithm


void print_Mat(int* M){

  int i, j;
  for(i = 0; i < N; ++i){
    for(j = 0; j < N; ++j){
      printf("%d ", M[i*N+j]);
    }
    printf("\n");
  }


}


int main(){

  //allocate and populate arrays
  int i, j, k;
  time_t t;
  struct timespec start, finish;
  double fin_time;
  srand((unsigned) time(&t));
  
  int* A_mat = (int*) malloc(N*N*sizeof(int));
  int* B_mat = (int*) malloc(N*N*sizeof(int));
  int* C_mat = (int*) malloc(N*N*sizeof(int));

  for(i = 0; i  < N; ++i){
    for(j=0; j < N; ++j){
      A_mat[i*N+j] = rand() % DOMAIN +1;
      B_mat[i*N+j] = rand() % DOMAIN +1;
    }
  }


  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i = 0; i < N; ++i){
    for(j = 0; j < N; ++j){
      for(k = 0; k < N; ++k){
        C_mat[i*N+j] += A_mat[i*N+k] * B_mat[k*N+j];
      }
    }
  }
  
  clock_gettime(CLOCK_MONOTONIC, &finish);
  fin_time = (finish.tv_sec - start.tv_sec);
  fin_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  

  printf("Time to complete: %f\n", fin_time);

  // Multiply arrays
  free(A_mat);
  free(B_mat);
  free(C_mat);
  
  return 0;
}
