#pragma D option quiet

pid$target:libmpi:MPI_Send:entry
/uid==1015/
{
	self->sendStart = timestamp;
}



pid$target:libmpi:MPI_Send:return
/uid==1015/
{
	@count["sends", pid] = count();
	@ft["total time(ns) sending", pid] = sum(timestamp - self->sendStart);
	self->sendStart = 0;
}



pid$target:libmpi:MPI_Recv:entry
/uid==1015/
{
	self->receiveStart = timestamp;
}



pid$target:libmpi:MPI_Recv:return
/uid==1015/
{
	@count["receives", pid] = count();
	@ft["total time(ns) receiving", pid] = sum(timestamp - self->receiveStart);
	self->receiveStart = 0;
}



pid$target:libmpi:MPI_Scatter:entry
/uid==1015/
{
	self->scatterStart = timestamp;
}



pid$target:libmpi:MPI_Scatter:return
/uid==1015/
{
	@count["scaters", pid] = count();
	@ft["total time(ns) scattering", pid] = sum(timestamp - self->scatterStart);
	self->scatterStart = 0;
}



pid$target:libmpi:MPI_Gather:entry
/uid==1015/
{
	self->gatherStart = timestamp;
}



pid$target:libmpi:MPI_Gather:return
/uid==1015/
{
	@count["gathers", pid] = count();
	@ft["total time(ns) gathering", pid] = sum(timestamp - self->gatherStart);
	self->gatherStart = 0;
}



pid$target:libmpi:MPI_Bcast:entry
/uid==1015/
{
	self->bcastStart = timestamp;
}



pid$target:libmpi:MPI_Bcast:return
/uid==1015/
{
	@count["broadcasts", pid] = count();
	@ft["total time(ns) broadcasting", pid] = sum(timestamp - self->bcastStart);
	self->bcastStart = 0;
}


histogram_equalization$target:::after-read
{
	self->masterStart = timestamp;
}

histogram_equalization$target:::before-write
{
	printf("total equalization time(ns):%d\n", (timestamp - self->masterStart));
}