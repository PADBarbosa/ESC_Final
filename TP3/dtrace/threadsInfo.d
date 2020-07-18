proc:::lwp-create
/pid == $target/
{
	printf("Create:%s, %d  tid: %d %d\n", stringof(args[1]->pr_fname), args[0]->pr_lwpid, tid, timestamp);
}


proc:::lwp-start
/pid == $target/
{
	printf("Start: %d %d\n", tid, timestamp);
}

proc:::lwp-exit
/pid == $target/
{
	printf("Exit: %d %d\n", tid, timestamp);
}