cd EngSisComp/NPB3.3.1-MZ/NPB3.3-MZ-SER-O3-xHost/

module load intel/2020
module load intel/2019
module load gcc/5.3.0

make clean
make suite

mpstat -P ALL 2 > PT2/mpstat_sp_641_ifort_A.txt &
vmstat 2 > PT2/vmstat_sp_641_ifort_A.txt &
iostat -d 2 > PT2/iostat_sp_641_ifort_A.txt &

./bin/sp-mz.A.x

>&2 jobs

kill %1
kill %2
kill %3

wait

>&2 jobs
############################################################


mpstat -P ALL 2 > PT2/mpstat_sp_641_ifort_B.txt &
vmstat 2 > PT2/vmstat_sp_641_ifort_B.txt &
iostat -d 2 > PT2/iostat_sp_641_ifort_B.txt &

./bin/sp-mz.B.x

>&2 jobs

kill %1
kill %2
kill %3

wait

>&2 jobs
############################################################


mpstat -P ALL 2 > PT2/mpstat_sp_641_ifort_C.txt &
vmstat 2 > PT2/vmstat_sp_641_ifort_C.txt &
iostat -d 2 > PT2/iostat_sp_641_ifort_C.txt &

./bin/sp-mz.C.x

>&2 jobs

kill %1
kill %2
kill %3

wait

>&2 jobs
