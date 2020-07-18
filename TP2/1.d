syscall::openat:entry
{

    self->execname = execname;
    self->pid = pid;
    self->uid = uid;
    self->gid = gid;
    self->path = copyinstr(arg1);

}


syscall::openat:entry
/(arg2 & 1) == 0 && (arg2 & 2) == 0/
{

    self->flags = "O_RDONLY";

}


syscall::openat:entry
/(arg2 & 1) == 1 && (arg2 & 2) == 0/
{

    self->flags = "O_WRONLY";

}


syscall::openat:entry
/(arg2 & 2) == 2/
{

    self->flags = "O_RDWR";

}


syscall::openat:entry
/(arg2 & 8) == 8/
{

    self->flags = strjoin(self->flags, ", O_APPEND");

}


syscall::openat:entry
/(arg2 & 256) == 256/
{

    self->flags = strjoin(self->flags, ", O_CREAT");

}



syscall::openat:return
/* /(self->uid == 1015) && (strstr(self->path, "etc/") != NULL)/ */
{
printf("\nNome do executável: %s \nPID do processo: %d \nUID do grupo: %d \nGID do grupo: %d \nPath: %s\nFlags:%s \nValor de retorno: %d \n", 
    self->execname, 
    self->pid, 
    self->uid, 
    self->gid,
    self->path,
    self->flags,
    arg0
    );

}
