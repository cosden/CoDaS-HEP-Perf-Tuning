/******************************************************************************************************
  July 10, 2017

  Ian A. Cosden
  Princeton University
  icosden@princeton.edu

  Sample Matrix-Matrix multiplication code for CoDaS-HEP Summer School 

  Purpose: 
     To use as a example to profile with performance tuning tools such as VTune.  The code does
     not do anything useful and is for illustrative/educational use only.  It is not meant to be
     exhaustive or demostrating optimal matrix-matrix multiplication techniques. 

  Description:
     Code generates two matrices with random numbers.  They are stored in 2D arrays (named A and B) as 
     well as 1D arrays (a and b).  There are 4 functions that multiply A and B and store the result in 
     matrix C (or c in the 1D case).  It is possible to set the percentage of time each function is 
     called via the command line. 

  Command line arguments: 
      N <threshold 1> <threshold 2> <threshold 3>
  
      N: size of NxN matrix
      **optional**:
      thresholds: 4 functions are distributed between 0 and 1.  
      mm1 is called between 0 and <threshold 1>
      mm2 is called between <threshold 1> and <threshold 2>
      mm3 is called between <threshold 2> and <threshold 3>
      mm4 is called between <threshold 3> and 1.
      
      Example:
        to call only mm1 use 1.0 0   0
        to call only mm2 use 0   1.0 1.0
        to call only mm3 use 0   0   1.0
        to call only mm4 use 0   0   0
      
*******************************************************************************************************/

#include "mm.h"


int main(int argc, char *argv[])
{

	int matrix_size; //N*N matrix
	double thresh_1, thresh_2, thresh_3;

	//read command line input
	//set various paramaters
	if(argc<2) {
		cout<<"ERROR: expecting integer matrix size, i.e., N for NxN matrix"<<endl;
		exit(1);
	}
	else {
		matrix_size=atoi(argv[1]);
	}
	
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
	cout<<"using matrix size:"<<matrix_size<<endl;
	
	mkl_set_num_threads(1); //needed to prevent mkl (mm4) from grabbing all available cores

	
	double **A, **B, **C; //2D arrays
	double *a, *b, *c;    //equivalent 1D arrays
	
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
	
	//initialize values crudely between 0 and 1
	//(we don't really care what they are)
	for (int i=0; i<matrix_size; i++) {
		idx=i*matrix_size;
		for (int j = 0 ; j < matrix_size; j++) {
			A[i][j]=((double) rand() / (RAND_MAX));
			B[i][j]=((double) rand() / (RAND_MAX));
			C[i][j]=0.0;
			a[idx+j]=A[i][j];
			b[idx+j]=B[i][j];
			c[idx+j]=0.0;
		}
	}
	
 
	int max_iters=50;  //number of times to call a matrix-matrix (mm) function
	double random_choice; //random number from rng
	srand (time(NULL));
	
	//Depending on random number and earlier set thresholds call a matrix-multiplication
	//function mm1,mm2,mm3,mm4.  This is done to purposely obfuscate the "hotspots"
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
