/*pid$target:histogram_equalization:_Z8parallelihiPh:entry*/
/*pid$target:histogram_equalization:_Z10sequentialhiPh:entry*/

pid$target:histogram_equalization:sequential:entry
{
    equalizing = 1;
}

pid$target:histogram_equalization:sequential:return
{
    equalizing = 0;
}


pid$target:histogram_equalization:parallel:entry
{
    equalizing = 1;
}

pid$target:histogram_equalization:parallel:return
{
    equalizing = 0;
}

sched:::on-cpu
/pid == $target && equalizing/
{
    self->ts = timestamp;

    printf("Thread %d started running on CPU %d\n", tid, cpu);
}

sched:::off-cpu
/self->ts && equalizing/
{
    printf("Thread %d stopped running on CPU %d\n", tid, cpu);

    @sum[tid,cpu] = sum(timestamp - self->ts);
    @hist[tid] = quantize(timestamp - self->ts);

    self->ts = 0;
}

