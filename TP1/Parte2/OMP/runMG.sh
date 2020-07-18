cd ~/NPB3.3.1/NPB3.3-SER

module load intel/2020
module load intel/2019
module load gcc/5.3.0

export OMP_NUM_THREADS=32

mpstat -P ALL 1 > PT2/mpstat_mg_A.txt &
vmstat 1 > PT2/vmstat_mg__A.txt &
iostat -mdx 1 > PT2/iostat_mg_A.txt &
free -m -s 1 > PT2/free_mg_A.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_mg_A.txt &


./bin/mg.A.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs
############################################################


mpstat -P ALL 1 > PT2/mpstat_mg_B.txt &
vmstat 1 > PT2/vmstat_mg__B.txt &
iostat -mdx 1 > PT2/iostat_mg_B.txt &
free -m -s 1 > PT2/free_mg_B.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_mg_B.txt &


./bin/mg.B.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs
############################################################


mpstat -P ALL 1 > PT2/mpstat_mg_C.txt &
vmstat 1 > PT2/vmstat_mg__C.txt &
iostat -mdx 1 > PT2/iostat_mg_C.txt &
free -m -s 1 > PT2/free_mg_C.txt &
#sar -n TCP,ETCP,DEV 1 > PT2/sar_mg_C.txt &


./bin/mg.C.x

>&2 jobs

kill %1
kill %2
kill %3
kill %4
#kill %5


wait

>&2 jobs