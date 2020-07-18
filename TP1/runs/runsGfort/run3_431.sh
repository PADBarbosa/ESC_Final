cd EngSisComp/NPB3.3.1-MZ/NPB3.3-MZ-OMP-gfortran/

module load intel/2020
module load intel/2019
module load gcc/5.3.0

make clean
make suite

export OMP_NUM_THREADS=1
echo "1 Threads"

./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x


export OMP_NUM_THREADS=2
echo "2 Threads"

./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x


export OMP_NUM_THREADS=4
echo "4 Threads"
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x


export OMP_NUM_THREADS=8
echo "8 Threads"
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x


export OMP_NUM_THREADS=16
echo "16 Threads"
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x


export OMP_NUM_THREADS=24
echo "24 Threads"
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x
./bin/bt-mz.B.x