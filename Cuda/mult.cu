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

void print_Mat(int* M);

void* multiply( void* arg );

__global__ void multiply(int* matA, int* matB, int* matC, int size, int Threads){

  //int col= blockIdx.x * blockDim.x + threadIdx.x, i, j, k;
  int i,j,k;
  int row= blockIdx.y * blockDim.y + threadIdx.y;

  int work = N/Threads;

  for(i = row*work; i < row*work+work; ++i){
    for(j = 0; j < N; ++j){
      matC[i*N+j] = 0;
      for(k = 0; k < N; ++k){
        matC[i*N+j] += matA[i*N+k] * matB[k*N+j];
      }
    }
  }
  return;

}


int main(){

  //allocate and populate arrays
  int i, j;
  time_t t;

  //Cuda variables
  int* device_MatA, *device_MatB, *device_MatC;

  //Time variables
  struct timespec start, finish;
  double fin_time;
  srand((unsigned) time(&t));
  
  //allocate and populate arrays with random numbers
  A_mat = (int*)malloc(N * N * sizeof(int));
  B_mat = (int*)malloc(N * N * sizeof(int));
  C_mat = (int*)malloc(N * N * sizeof(int));

  for(i = 0; i  < N; ++i){
    for(j=0; j < N; ++j){
      A_mat[i*N + j] = rand() % DOMAIN +1;
      B_mat[i*N + j] = rand() % DOMAIN +1;
    }
  }

  cudaMalloc(&device_MatA, N*N*sizeof(int));
  cudaMalloc(&device_MatB, N*N*sizeof(int));
  cudaMalloc(&device_MatC, N*N*sizeof(int));
  
  cudaMemcpy(device_MatA, A_mat, N*N*sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(device_MatB, B_mat, N*N*sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(device_MatC, C_mat, N*N*sizeof(int), cudaMemcpyHostToDevice);

  clock_gettime(CLOCK_MONOTONIC, &start);

  // -- 1 thread per cell multiply<<<1, dim3(4,4)>>>(deviceA_ptr, deviceB_ptr, deviceC_ptr);
  multiply<<<1, dim3(1, THREADS)>>>(device_MatA, device_MatB, device_MatC, N, THREADS);
  cudaDeviceSynchronize();

  cudaMemcpy(C_mat, device_MatC, N*N*sizeof(int), cudaMemcpyDeviceToHost);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  fin_time = (finish.tv_sec - start.tv_sec);
  fin_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  //print_Mat(A_mat);
  //print_Mat(B_mat);

  /*
  print_Mat(A_mat);
  printf("\n\n");
  print_Mat(B_mat);
  printf("\n\n");
  print_Mat(C_mat);
*/
  printf("Time to complete: %f\n", fin_time);

  // Multiply arrays
  free(A_mat);
  free(B_mat);
  free(C_mat);
  
  cudaFree(device_MatA);
  cudaFree(device_MatB);
  cudaFree(device_MatC);
  return 0;
}


void print_Mat(int* M){

  int i, j;
  for(i = 0; i < N; ++i){
    for(j = 0; j < N; ++j){
      printf("%d ", M[i*N+j]);
    }
    printf("\n");
  }
}

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

  return NULL;

}

