cd EngSisComp/NPB3.3.1-MZ/NPB3.3-MZ-OMP-ifort/

module load intel/2020
module load intel/2019
module load gcc/5.3.0

export OMP_NUM_THREADS=32

make clean
make suite

mpstat -P ALL 1 > PT2/mpstat_sp_D.txt &
vmstat 1 > PT2/vmstat_sp_D.txt &
iostat -mdx 1 > PT2/iostat_sp_D.txt &
free -m -s 1 > PT2/free_sp_D.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_sp_D.txt &


./bin/sp-mz.D.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs
