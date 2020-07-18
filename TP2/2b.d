#pragma D option quiet
/*dtrace -s 2b.d 5*/
BEGIN{
    APPEND = 8;
    CREAT = 256;    
}


syscall::openat:entry
{

        @attempts[execname, pid, "opens"] = count();


        creation_flags = "";

        ((arg2 & CREAT) != CREAT) ? 
            creation_flags = "open existing file" : 0;

        ((arg2 & CREAT) == CREAT) ? 
            creation_flags = "create new file" : 0;

        @attempts[execname, pid, creation_flags] = count();

}


syscall::openat*:return
/arg0 >= 0/
{

        @attempts[execname, pid, "successful"] = count();

}


tick-$1s
{
        printf("\n  execname                                                pid  action                                                       amount\n");
        printf("%Y\n", walltimestamp);
        printa(@attempts);
}
