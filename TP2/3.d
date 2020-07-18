#pragma D option quiet


syscall:::entry
/pid == $target/
{
        self->ts = timestamp;
        @totalCount[probefunc] = count();
}


syscall:::return
/pid == $target/
{
        @totalTime[probefunc] = sum(timestamp - self->ts);
}

END
{
        printa(@totalTime, @totalCount);
}
