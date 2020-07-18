ODIR = build
SDIR = src

all: $(ODIR)/histogram_equalization.o $(ODIR)/pgmb_io.o $(ODIR)/user_probes.o
	g++ -std=c++11 -O3 -o histogram_equalization $(ODIR)/histogram_equalization.o $(ODIR)/pgmb_io.o $(ODIR)/user_probes.o  -lm -fopenmp -lpthread

$(ODIR)/histogram_equalization.o: $(SDIR)/histogram_equalization.cpp $(SDIR)/barrier.cpp $(SDIR)/user_probes.h
	dtrace -o $(SDIR)/user_probes.h -h -s $(SDIR)/user_probes.d
	g++ -std=c++11 -O3 -c -o $(ODIR)/histogram_equalization.o $(SDIR)/histogram_equalization.cpp -fopenmp -lpthread

$(ODIR)/pgmb_io.o: $(SDIR)/pgmb_io.c
	g++ -std=c++11 -O3 -c -o $(ODIR)/pgmb_io.o $(SDIR)/pgmb_io.c


$(ODIR)/user_probes.o: $(SDIR)/user_probes.d
	dtrace -G -s $(SDIR)/user_probes.d -o $(ODIR)/user_probes.o $(ODIR)/histogram_equalization.o


clean:
	rm $(ODIR)/*.o histogram_equalization
