#include <stdbool.h>
#include "pgmb_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <omp.h>
#include <pthread.h>
#include "mm_malloc.h"
#include "user_probes.h"



double start, end;




void startTimer(){
	start = omp_get_wtime();
}

void stopTimer(){
	end = omp_get_wtime();
	double time = ((double) (end - start));
	printf("%f segundos\n", time);
}



unsigned char* sequential(unsigned char maxgray, int size, unsigned char* restrict data){

	int* histogram = malloc(sizeof(int)*maxgray);
	int* normalized = malloc(sizeof(int)*maxgray);
	memset(histogram, 0, sizeof(int)*maxgray);
	memset(normalized, 0, sizeof(int)*maxgray);



    if(HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED()){
        HISTOGRAM_EQUALIZATION_EQUALIZATION_START(size);
    }

	for(int i=0; i < size; i++) {
		histogram[data[i]]++;
	}

    if(HISTOGRAM_EQUALIZATION_COMPUTING_DONE_ENABLED()){
       HISTOGRAM_EQUALIZATION_COMPUTING_DONE(size);
    }

	int cumulativeDistribution = 0;
	double constant = (double)maxgray / size;

    

	for(int i=0; i <= maxgray; i++) {
		cumulativeDistribution += histogram[i];
		normalized[i] = round(cumulativeDistribution * constant); 
	}

    if(HISTOGRAM_EQUALIZATION_NORMALIZING_DONE_ENABLED()){
        HISTOGRAM_EQUALIZATION_NORMALIZING_DONE(size);
    }

	for (int i = 0; i < size; i++) {
		data[i] = normalized[data[i]];
	}

    if(HISTOGRAM_EQUALIZATION_MAPPING_DONE_ENABLED()){
        HISTOGRAM_EQUALIZATION_MAPPING_DONE(size);
    }
	return data;
}



unsigned char* parallel(unsigned char maxgray, int size, unsigned char* data){
	
	int* histogram = malloc(sizeof(int)*maxgray);
	int* threadHistogram;
	
	memset(histogram, 0, sizeof(int)*maxgray);

    if(HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED()){
        HISTOGRAM_EQUALIZATION_EQUALIZATION_START(size);
    }

	#pragma omp parallel private(threadHistogram) shared(histogram) 
	{	

		int chunck_size = size / (omp_get_num_threads());
		threadHistogram = malloc(sizeof(int) * maxgray);
		memset(threadHistogram, 0, sizeof(int)*maxgray);

		#pragma omp for schedule(static,chunck_size)
		for(int i=0; i < size; i++) {
			threadHistogram[ data[i] ]++;
		}
		
		#pragma omp critical
		{
			for(int i=0; i <= maxgray; i++) {
				histogram[i] += threadHistogram[i];
			}
		}
	}

    if(HISTOGRAM_EQUALIZATION_COMPUTING_DONE_ENABLED()){
       HISTOGRAM_EQUALIZATION_COMPUTING_DONE(size);
    }


	int cumulativeDistribution = 0;
	int *cumulativeDistributionArray = malloc(sizeof(int)*maxgray);
	
	for(int i=0; i <= maxgray; i++) {
		cumulativeDistribution += histogram[i];	
		cumulativeDistributionArray[i] = cumulativeDistribution;
	}



	int* normalized = malloc(sizeof(int)*maxgray);
	memset(normalized, 0, sizeof(int)*maxgray);
	double constant = (double)maxgray / size;

	#pragma omp parallel
	{
		int chunck_size = size / (omp_get_num_threads());
		#pragma omp for schedule(static,chunck_size) 
		for(int i=0; i <= maxgray; i++) {
			normalized[i] = round(cumulativeDistributionArray[i] * constant);
		}
    }


    if(HISTOGRAM_EQUALIZATION_NORMALIZING_DONE_ENABLED()){
        HISTOGRAM_EQUALIZATION_NORMALIZING_DONE(size);
    }



	#pragma omp parallel
    {
        //Repetido para facilitar a utilização da  probe NORMALIZING_DONE
        int chunck_size = size / (omp_get_num_threads());

        #pragma omp for schedule(static,chunck_size)  
    	for(int i = 0; i < size; i++) {
    		data[i] = normalized[data[i]];
    	}		

    }    
	

    if(HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED()){
        HISTOGRAM_EQUALIZATION_EQUALIZATION_START(size);
    }

	return data;
}


/*
Exemplos:
./histogram_equalization <image_name> <program_version>
./histogram_equalization buffalo.pgm s       Executa o programa com a versão sequencial
./histogram_equalization buffalo.pgm p       Executa o programa com a versão paralela
*/

int main(int argc, char *argv[]) {

	if(argc < 2){
		printf("Argumentos inválidos!\n");
		return -1;
	}

	int xsize, ysize, img_colors;
	unsigned char maxgray;
	unsigned char *data;
	unsigned char *newData;

	char* imagem;
	imagem = argv[1];

	pgmb_read(imagem, &xsize, &ysize, &maxgray, &data);
    //xsize = 10000;
    //ysize = 10000;
    //maxgray = 255;
    //data = malloc(sizeof(char)*xsize*ysize);

	int size = xsize*ysize; 

    printf("SIZE: %d\n", size);

	


	if(strcmp(argv[2], "s") == 0){
		printf("--------Sequencial--------\n");
		startTimer();
		newData = sequential(maxgray, size, data);
		stopTimer();
	}
	else if(strcmp(argv[2], "p") == 0){
		printf("--------Paralela--------\n");
		startTimer();
		newData = parallel(maxgray, size, data);
		stopTimer();
	}

    else if(strcmp(argv[2], "io") == 0){
        printf("--------Only read and write--------\n");
    }

	pgmb_write( "output.pgm", xsize, ysize, newData, maxgray);

	return 0;
}