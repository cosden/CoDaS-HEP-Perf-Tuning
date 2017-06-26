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


int main(int argc, char *argv[])
{

  int matrix_size;

  if(argc<2) {
    cout<<"ERROR: expecting integer matrix size, i.e., N for NxN matrix"<<endl;
    exit(1);
  }
  else {
    matrix_size=atoi(argv[1]);
  }

  double thresh_1, thresh_2, thresh_3;
  if(argc<5) {
    thresh_1=0.1;
    thresh_2=0.3; 
    thresh_3=0.6;
    cout<<"Using default thresholds: ";
  }
  else {
    thresh_1=atof(argv[2]); 
    thresh_2=atof(argv[3]); 
    thresh_3=atof(argv[4]); 
    cout<<"Using user supplied thresholds: ";
  }

  cout<<thresh_1<<" "<<thresh_2<<" "<<thresh_3<<endl;

  mkl_set_num_threads(1);
  cout<<"using matrix size:"<<matrix_size<<endl;

  double **A, **B, **C;
  double *a, *b, *c;

  A = new double*[matrix_size];
  B = new double*[matrix_size];
  C = new double*[matrix_size];
  a = new double[matrix_size*matrix_size];
  b = new double[matrix_size*matrix_size];
  c = new double[matrix_size*matrix_size];
  

  for (int i = 0 ; i < matrix_size; i++) {
    A[i] = new double[matrix_size];
    B[i] = new double[matrix_size];
    C[i] = new double[matrix_size];
  }
  
  int idx;
  for (int i=0; i<matrix_size; i++)
    {
      idx=i*matrix_size;
      for (int j = 0 ; j < matrix_size; j++)
	{
	  A[i][j]=((double) rand() / (RAND_MAX));
	  B[i][j]=((double) rand() / (RAND_MAX));
	  C[i][j]=0.0;
	  a[idx+j]=A[i][j];
	  b[idx+j]=B[i][j];
	  c[idx+j]=0.0;
	}
    }
  print_check(A, matrix_size);


 
  timeval t1, t2, t;

//---------------------------------------------------------------------
//standard implementation
//---------------------------------------------------------------------
/*  gettimeofday(&t1, NULL);
  
  mm1(A,B,C,matrix_size);
  print_check(C,matrix_size);
  zero_result(C,matrix_size);
  print_check(C,matrix_size);
  
  gettimeofday(&t2, NULL);
  print_elapsed_time(t1,t2,"mm1");
  gettimeofday(&t1, NULL);
  
  mm2(A,B,C,matrix_size);
  print_check(C,matrix_size);
  zero_result(C,matrix_size);
  
  gettimeofday(&t2, NULL);
  print_elapsed_time(t1,t2,"mm2");
  gettimeofday(&t1, NULL);
  
  mm3(a,b,c,matrix_size);
  print_check_1D(c,matrix_size);
  zero_result_1D(c,matrix_size);
  
  gettimeofday(&t2, NULL);
  print_elapsed_time(t1,t2,"mm3");
  gettimeofday(&t1, NULL);
  
  mm4(a,b,c,matrix_size);
  print_check_1D(c,matrix_size);
  zero_result_1D(c,matrix_size);
  
  gettimeofday(&t2, NULL);
  print_elapsed_time(t1,t2,"mm4");
*/
  
  
  int max_iters=50;
  double random_choice;

  for (int r=0; r < max_iters; r++) {
    random_choice = ((double) rand() / (RAND_MAX));
    if (random_choice<thresh_1) {
      zero_result(C,matrix_size);
      mm1(A,B,C,matrix_size);
    }
    else if (random_choice>=thresh_1 && random_choice<thresh_2) {
      zero_result(C,matrix_size);
      mm2(A,B,C,matrix_size);
    }
    else if (random_choice>=thresh_2 && random_choice<thresh_3) {
      zero_result_1D(c,matrix_size);
      mm3(a,b,c,matrix_size);
    }
    else {
      zero_result_1D(c,matrix_size);
      mm4(a,b,c,matrix_size);
    }
  }

    
  
  
  for (int i = 0 ; i < matrix_size; i++)  {
    delete A[i];
    delete B[i];
    delete C[i];
  }
  
  
  delete A;
  delete B;
  delete C;
  
  
  return 0;
}
