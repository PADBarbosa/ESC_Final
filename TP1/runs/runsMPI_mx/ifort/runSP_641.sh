module load intel/openmpi_mx/1.8.4

cd ~/EngSisComp/NPB3.3.1-MZ/NPB3.3-MZ-MPI-ifort/

make clean
make suite

echo "2 processos"
export OMP_NUM_THREADS=32
mpirun -env I_MPI_DEBUG 4 -np 2 -perhost 1  bin/sp-mz.B.2
mpirun -env I_MPI_DEBUG 4 -np 2 -perhost 1  bin/sp-mz.B.2
mpirun -env I_MPI_DEBUG 4 -np 2 -perhost 1  bin/sp-mz.B.2
mpirun -env I_MPI_DEBUG 4 -np 2 -perhost 1  bin/sp-mz.B.2
mpirun -env I_MPI_DEBUG 4 -np 2 -perhost 1  bin/sp-mz.B.2


echo "4 processos"
export OMP_NUM_THREADS=16
mpirun -env I_MPI_DEBUG 4 -np 4 -perhost 2  bin/sp-mz.B.4
mpirun -env I_MPI_DEBUG 4 -np 4 -perhost 2  bin/sp-mz.B.4
mpirun -env I_MPI_DEBUG 4 -np 4 -perhost 2  bin/sp-mz.B.4
mpirun -env I_MPI_DEBUG 4 -np 4 -perhost 2  bin/sp-mz.B.4
mpirun -env I_MPI_DEBUG 4 -np 4 -perhost 2  bin/sp-mz.B.4


echo "8 processos"
export OMP_NUM_THREADS=8
mpirun -env I_MPI_DEBUG 4 -np 8 -perhost 4  bin/sp-mz.B.8
mpirun -env I_MPI_DEBUG 4 -np 8 -perhost 4  bin/sp-mz.B.8
mpirun -env I_MPI_DEBUG 4 -np 8 -perhost 4  bin/sp-mz.B.8
mpirun -env I_MPI_DEBUG 4 -np 8 -perhost 4  bin/sp-mz.B.8
mpirun -env I_MPI_DEBUG 4 -np 8 -perhost 4  bin/sp-mz.B.8


echo "16 processos"
export OMP_NUM_THREADS=4
mpirun -env I_MPI_DEBUG 4 -np 16 -perhost 8  bin/sp-mz.B.16
mpirun -env I_MPI_DEBUG 4 -np 16 -perhost 8  bin/sp-mz.B.16
mpirun -env I_MPI_DEBUG 4 -np 16 -perhost 8  bin/sp-mz.B.16
mpirun -env I_MPI_DEBUG 4 -np 16 -perhost 8  bin/sp-mz.B.16
mpirun -env I_MPI_DEBUG 4 -np 16 -perhost 8  bin/sp-mz.B.16