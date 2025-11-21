#!/bin/bash 
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=5
#SBATCH --time=1:00:00
#SBATCH --job-name merge
#SBATCH --output=merge-%j.txt
#SBATCH --mail-type=FAIL
 
cd $SLURM_SUBMIT_DIR
 
module load gcc
module load gmp
module load openmpi
 
mpirun ~/merge $1
