#!/bin/bash
#SBATCH -N 1 #one node
#SBATCH -n 1 #one task
#SBATCH -t 00:05:00 #maximum wall time of 5 minutes
#SBATCH --reservation=codas


#load environment modules that correspond to the build environment
#and puts VTune in path

module load intel intel-vtune


#run command line VTune, with the hotspots analysis on the executable mm.out for a
# 500x500 matrix 

amplxe-cl -collect hotspots -- ./mm.out 500
