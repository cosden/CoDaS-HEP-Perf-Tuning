#!/bin/bash
#SBATCH -N 1 #one node
#SBATCH -n 1 #one task
#SBATCH -t 00:05:00 #maximum wall time of 5 minutes
#SBATCH --reservation=class_44

module load intel intel-vtune

amplxe-cl -collect hotspots -- ./mm.out 500
