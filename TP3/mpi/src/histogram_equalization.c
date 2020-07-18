#include <stdbool.h>
#include "pgmb_io.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "user_probes.h"



void master(char* imagem, int nprocesses) {
	MPI_Status status;

	int xsize, ysize, img_colors;
	unsigned char maxgray;
	unsigned char *data;
	unsigned char *newData;


	//Carregar imagem
	pgmb_read(imagem, &xsize, &ysize, &maxgray, &data);


	int size = xsize*ysize; 

    if(HISTOGRAM_EQUALIZATION_AFTER_READ_ENABLED()){
        HISTOGRAM_EQUALIZATION_AFTER_READ(size);
    }

	int chunck_size = size / nprocesses;

	double startTime = MPI_Wtime();

	//Enviar tamanho dos pedaços e maxgray
	int metadados[2] = {chunck_size, (int) maxgray};
	MPI_Bcast(metadados, 2, MPI_INT, 0, MPI_COMM_WORLD);

	unsigned char *data_chunck = malloc(sizeof(unsigned char)*chunck_size);
	MPI_Scatter(data, chunck_size, MPI_UNSIGNED_CHAR, data, chunck_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	
	int* histogram;
	histogram = malloc(sizeof(int)*(maxgray+1));
	memset(histogram, 0, sizeof(int)*(maxgray+1));


	for(int i=0; i < chunck_size; i++) {
		histogram[ data[i] ]++;
	}


	int* localHistogram;
	localHistogram = malloc(sizeof(int)*(maxgray+1));
	memset(localHistogram, 0, sizeof(int)*(maxgray+1));

	//Receber os histogramas locais e juntar num só
	for (int i = 1; i < nprocesses; i++) {
		MPI_Recv(localHistogram, maxgray+1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status);
		for(int  j = 0; j <= maxgray; j++) {
			histogram[j] += localHistogram[j];
		}
	}


	int cumulativeDistribution = 0;
	int* normalized = malloc(sizeof(int)*maxgray);
	memset(normalized, 0, sizeof(int)*maxgray);
	double constant = (double)maxgray / size;

	//Normalizar histograma
	for(int i = 0; i <= maxgray; i++) {
		cumulativeDistribution += histogram[i];	
		normalized[i] = round(cumulativeDistribution * constant);
	}

	//Broadcast do histograma normalizado
	MPI_Bcast(normalized, maxgray+1, MPI_INT, 0, MPI_COMM_WORLD);


	for(int i = 0; i < chunck_size; i++) {
		data[i] = normalized[data[i]];
	}


	unsigned char *tempdata = malloc(sizeof(unsigned char)*chunck_size);

	//Receber informação relativa à imagem resultado de cada um dos slaves
	MPI_Gather(data, chunck_size, MPI_UNSIGNED_CHAR, data, chunck_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);


	if(HISTOGRAM_EQUALIZATION_BEFORE_WRITE_ENABLED()){
        HISTOGRAM_EQUALIZATION_BEFORE_WRITE(size);
    }

	double endTime = MPI_Wtime();

	printf("Time:%lf\n", endTime - startTime);

	pgmb_write( "output.pgm", xsize, ysize, data, maxgray);

	MPI_Finalize ();
}


void slave() {
	MPI_Status status;

	//Receber tamamho do pedaço a processar e maxgray
	int metadados[2];
	MPI_Bcast(metadados, 2, MPI_INT, 0, MPI_COMM_WORLD);
	int chunck_size = metadados[0];
	unsigned char maxgray = (unsigned char) metadados[1];



	int* histogram;
	histogram = malloc(sizeof(int)*(maxgray+1));
	memset(histogram, 0, sizeof(int)*(maxgray+1));

	unsigned char *data = malloc(sizeof(unsigned char)*chunck_size);

	//Receber chunck da imagem a processar
	MPI_Scatter(NULL, chunck_size, MPI_UNSIGNED_CHAR, data, chunck_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

	for(int i=0; i < chunck_size; i++) {
		histogram[ data[i] ]++;
	}

	//Envia histograma relativo ao chunk da imagem que processou
	MPI_Send(histogram, maxgray+1, MPI_INT, 0, 0, MPI_COMM_WORLD);


	int* normalized;
	normalized = malloc(sizeof(int)*(maxgray+1));
	
	//Recebe o histograma normalizado
	MPI_Bcast(normalized, maxgray+1, MPI_INT, 0, MPI_COMM_WORLD);


	for(int i = 0; i < chunck_size; i++) {
		data[i] = normalized[data[i]];
	}

	//Envia novos dados da imagem 
	MPI_Gather(data, chunck_size, MPI_UNSIGNED_CHAR, NULL, chunck_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);


	MPI_Finalize ();
}


/*
Para executar:
mpirun -np "nº de processos" -mca btl self,sm,tcp "nome do executável" "imagem em formato pgm" 

Exemplo:
mpirun -np 2 -mca btl self,sm,tcp histogram_equalization test.pgm
mpirun -np 5 -mca btl self,sm,tcp histogram_equalization test.pgm

*/
int main(int argc, char **argv) {
	int nprocesses; 
	int myrank; 


	MPI_Init (&argc, &argv); 

	MPI_Comm_size (MPI_COMM_WORLD, &nprocesses); 
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank); 

	if(nprocesses == 1) {
		printf("Erro: menos de 2 processos\n");
		MPI_Finalize ();
		return 0;
	}

	if(myrank == 0){
		master(argv[1], nprocesses);
	}
	else{
		slave();
	}

	return 0;
}