### CoDaS-HEP Summer School 2017
# Introduction to Performance Tuning & Optimization Tools


Sample Matrix-Matrix multiplication code for CoDaS-HEP Summer School

This is intended to use as a example to profile with performance tuning tools such as VTune.  The code does not do anything useful and is for illustrative/educational use only.  It is not meant to be exhaustive or demostrating optimal matrix-matrix multiplication techniques.

## Instructions for Running on Adroit

1. Log into Adroit with X11 forwarding enabled
`ssh -X <username>@adroit.princeton.edu`
1. Load environment modules  
`module load intel/17.0`  
`module load intel-vtune`
1. Build the example code  
  Don’t forget the `-g`  
  E.g. `icpc -g -mkl -O3 -xhost matmul_test.cpp -o mm.out`   
  Short test: `./mm.out 250` (250x250 matrix)   
1. Launch VTune GUI  
`amplxe-gui`


## Contact
Ian A. Cosden  
Princeton University  
icosden@princeton.edu

  
