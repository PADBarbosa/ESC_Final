ODIR = build
SDIR = src

all: $(ODIR)/histogram_equalization.o $(ODIR)/pgmb_io.o $(ODIR)/user_probes.o
	/usr/local/bin/mpicc -O3 -o histogram_equalization $(ODIR)/histogram_equalization.o $(ODIR)/pgmb_io.o $(ODIR)/user_probes.o -lm

$(ODIR)/histogram_equalization.o:  $(SDIR)/histogram_equalization.c
	dtrace -o $(SDIR)/user_probes.h -h -s $(SDIR)/user_probes.d
	/usr/local/bin/mpicc -O3 -c -o $(ODIR)/histogram_equalization.o $(SDIR)/histogram_equalization.c

$(ODIR)/pgmb_io.o: $(SDIR)/pgmb_io.c
	/usr/local/bin/mpicc -O3 -c -o $(ODIR)/pgmb_io.o $(SDIR)/pgmb_io.c

$(ODIR)/user_probes.o: $(SDIR)/user_probes.d
	dtrace -G -s $(SDIR)/user_probes.d -o $(ODIR)/user_probes.o $(ODIR)/histogram_equalization.o
	
run:
	/usr/local/bin/mpirun -np $(p) -mca btl self,sm,tcp histogram_equalization buffalo.pgm




clean:
	rm $(ODIR)/*.o histogram_equalization
