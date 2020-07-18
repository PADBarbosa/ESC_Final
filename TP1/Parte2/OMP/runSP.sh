cd EngSisComp/NPB3.3.1-MZ/NPB3.3-MZ-OMP-ifort/

module load intel/2020
module load intel/2019
module load gcc/5.3.0

export OMP_NUM_THREADS=32

make clean
make suite

mpstat -P ALL 1 > PT2/mpstat_sp_A.txt &
vmstat 1 > PT2/vmstat_sp__A.txt &
iostat -mdx 1 > PT2/iostat_sp_A.txt &
free -m -s 1 > PT2/free_sp_A.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_sp_A.txt &


./bin/sp-mz.A.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs
############################################################


mpstat -P ALL 1 > PT2/mpstat_sp_B.txt &
vmstat 1 > PT2/vmstat_sp__B.txt &
iostat -mdx 1 > PT2/iostat_sp_B.txt &
free -m -s 1 > PT2/free_sp_B.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_sp_B.txt &


./bin/sp-mz.B.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs
############################################################


mpstat -P ALL 1 > PT2/mpstat_sp_C.txt &
vmstat 1 > PT2/vmstat_sp__C.txt &
iostat -mdx 1 > PT2/iostat_sp_C.txt &
free -m -s 1 > PT2/free_sp_C.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_sp_C.txt &


./bin/sp-mz.C.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs