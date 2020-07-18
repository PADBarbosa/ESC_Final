/*pid$target:histogram_equalization:_Z8parallelihiPh:entry*/

proc:::lwp-create
/pid == $target/
{
    printf("%s, %d\n", stringof(args[1]->pr_fname), args[0]->pr_lwpid);
}

proc:::lwp-start
/pid == $target/
{
    self->threadStart = timestamp;
}


pid$target:histogram_equalization:parallel:entry
{
    equalizing = 1;
}

pid$target:histogram_equalization:parallel:return
{
    equalizing = 0;
}

plockstat$target:::mutex-spin
/pid == $target && equalizing/
{
        self->spinStart = timestamp;
}


plockstat$target:::mutex-acquire
/pid == $target && self->spinStart && equalizing/
{
        @time["total time(ns) waiting on mutexes:", tid] = sum((timestamp - self->spinStart)/1000000);
        self->spinStart = 0;
}



proc:::lwp-exit
/pid == $target && self->threadStart && equalizing/
{
    @time["total time(ns)", tid] = sum(timestamp - self->threadStart);
    self->threadStart = 0;
}
