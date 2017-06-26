#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <sys/time.h>
#include <omp.h>
#include <mkl.h>
using namespace std;



void mm1(double **A, double **B, double **C, int matrix_size) {
  for (int i = 0 ; i < matrix_size; i++) {
    for (int j = 0;  j < matrix_size; j++) {
      for (int k = 0; k < matrix_size; k++) {
	C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void mm2(double **A, double **B, double **C, int matrix_size) {

      for (int i = 0 ; i < matrix_size; i++) {
	for (int k = 0; k < matrix_size; k++) {
	  for (int j = 0;  j < matrix_size; j++) {
	    C[i][j] += A[i][k] * B[k][j];
	}
      }
    }
    
}

void mm3(double *a, double *b, double *c, int matrix_size) {

  for (int i=0 ; i<matrix_size; i++) {
    for (int j=0;  j<matrix_size; j++) {
      int idx = i*matrix_size;
      for (int k=0; k<matrix_size; k++) {
        c[idx+j] += a[idx+k]*b[k*matrix_size+j];
      }
    }
  }
}

void mm4(double *a, double *b, double *c, int matrix_size) {

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
              matrix_size,  matrix_size,  matrix_size, 1.0, a, matrix_size,
              b, matrix_size, 0.0, c, matrix_size);


}

void print_check(double **Z, int matrix_size) {
  
  cout<<Z[0][0]<<" "<<Z[1][1]<<" "<<Z[2][2]<<endl;
}

void print_check_1D(double *Z, int matrix_size) {
  
  cout<<Z[0]<<" "<<Z[matrix_size+1]<<" "<<Z[2*matrix_size+2]<<endl;
  
}



void zero_result(double **C, int matrix_size) {
  for (int i = 0 ; i < matrix_size; i++) {
    for (int j = 0;  j < matrix_size; j++) {
        C[i][j] = 0.0;
    }
  }  
}

void zero_result_1D(double *C, int matrix_size) {
  for (int i = 0 ; i < matrix_size*matrix_size; i++) {
        C[i] = 0.0;
    }
  }  

void print_elapsed_time(timeval t1, timeval t2, string s) {
  timeval t;
  
  timersub(&t2, &t1, &t);
  cout << t.tv_sec + t.tv_usec/1000000.0 << " Seconds -- "<< s << endl;
}
