module load gnu/openmpi_eth/2.0.0
module load gcc/5.3.0

cd ~/EngSisComp/NPB3.3.1-MZ/NPB3.3-MZ-MPI-gfortran/

make clean
make suite

echo "2 processos"
export OMP_NUM_THREADS=24
mpirun -np 2 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.2
mpirun -np 2 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.2
mpirun -np 2 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.2
mpirun -np 2 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.2
mpirun -np 2 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.2


echo "4 processos"
export OMP_NUM_THREADS=12
mpirun -np 4 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.4
mpirun -np 4 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.4
mpirun -np 4 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.4
mpirun -np 4 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.4
mpirun -np 4 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.4


echo "8 processos"
export OMP_NUM_THREADS=6
mpirun -np 8 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.8
mpirun -np 8 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.8
mpirun -np 8 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.8
mpirun -np 8 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.8
mpirun -np 8 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.8


echo "12 processos"
export OMP_NUM_THREADS=4
mpirun -np 12 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.12
mpirun -np 12 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.12
mpirun -np 12 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.12
mpirun -np 12 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.12
mpirun -np 12 --map-by node -report-bindings -mca btl self,sm,tcp bin/sp-mz.B.12