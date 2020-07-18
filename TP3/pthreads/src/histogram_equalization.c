#include <stdbool.h>
#include "pgmb_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <omp.h>
#include "mm_malloc.h"
#include "user_probes.h"

#include <pthread.h>


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


//------------------------------------------------------------------------------------------------------------------------------------------------



int* parallel_histogram; 
unsigned char parallel_maxgray; 
int parallel_size;
unsigned char* parallel_data;
int thread_count;

pthread_barrier_t barrier;

int* parallel_normalized;

int* parallel_cumulativeDistributionArray;

pthread_mutex_t mutex;


void* calculate_histogram(void* arg_thread_id){

	int* threadHistogram = malloc(parallel_maxgray*sizeof(int));
	memset(threadHistogram, 0, sizeof(int)*parallel_maxgray);

	long thread_id = (long) arg_thread_id;

	int thread_chunck_size = parallel_size / thread_count;
	int start = thread_chunck_size*thread_id;
	int end = start + thread_chunck_size;

	if(thread_id == thread_count-1){
		end = parallel_size;
	}

	for(int i = start; i < end; i++) {
		threadHistogram[ parallel_data[i] ]++;
	}

	pthread_mutex_lock(&mutex);
	for(int i=0; i <= parallel_maxgray; i++) {
		parallel_histogram[i] += threadHistogram[i];
	}
	pthread_mutex_unlock(&mutex);

}


void* cumulative_distribution(void* arg_thread_id){
	long thread_id = (long) arg_thread_id;

	int cumulativeDistribution = 0;

	if(thread_id == 0){
		for(int i=0; i <= parallel_maxgray; i++) {
			cumulativeDistribution += parallel_histogram[i]; 
			parallel_cumulativeDistributionArray[i] = cumulativeDistribution;
		}
	}

	pthread_barrier_wait(&barrier);


	int thread_chunck_size = parallel_maxgray/thread_count;
	int start = thread_chunck_size*thread_id;
	int end = start + thread_chunck_size;
	
	if(thread_id == thread_count-1){
		end = parallel_maxgray;
	}

	double constant = (double)parallel_maxgray / parallel_size;
	for(int i=start; i <= end; i++) {
		parallel_normalized[i] = round(parallel_cumulativeDistributionArray[i] * constant);
	}
}

void* map_new_values(void* arg_thread_id){
	long thread_id = (long) arg_thread_id;

	int thread_chunck_size = parallel_size / thread_count;
	int start = thread_chunck_size*thread_id;
	int end = start + thread_chunck_size;
	
	if(thread_id == thread_count-1){
		end = parallel_size;
	}
	
	for (int i = start; i < end; i++) {
		parallel_data[i] = parallel_normalized[parallel_data[i]];
	}

}



void* thread_func(void* arg_thread_id){
	long thread_id = (long) arg_thread_id;

	calculate_histogram(arg_thread_id);
	pthread_barrier_wait(&barrier);

	if(thread_id == 0 && HISTOGRAM_EQUALIZATION_COMPUTING_DONE_ENABLED()){
		HISTOGRAM_EQUALIZATION_COMPUTING_DONE(parallel_size);
	}

	cumulative_distribution(arg_thread_id);
	pthread_barrier_wait(&barrier);
	
	if(thread_id == 0 && HISTOGRAM_EQUALIZATION_NORMALIZING_DONE_ENABLED()){
		HISTOGRAM_EQUALIZATION_NORMALIZING_DONE(parallel_size);
	}

	map_new_values(arg_thread_id);
	pthread_barrier_wait(&barrier);
}


unsigned char* parallel(unsigned char maxgray, int size, unsigned char* data){

	parallel_maxgray = maxgray; 
	parallel_size = size;
	parallel_data = data;
	parallel_histogram =  malloc(maxgray*sizeof(int)); 
	pthread_mutex_init(&mutex, NULL);

	parallel_histogram = malloc(sizeof(int)*parallel_maxgray);
	parallel_normalized = malloc(sizeof(int)*parallel_maxgray);
	memset(parallel_histogram, 0, sizeof(int)*parallel_maxgray);
	memset(parallel_normalized, 0, sizeof(int)*parallel_maxgray);

	parallel_cumulativeDistributionArray = malloc(sizeof(int)*parallel_maxgray);

	pthread_barrier_init(&barrier, NULL, thread_count);

	

	if(HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED()){
		HISTOGRAM_EQUALIZATION_EQUALIZATION_START(parallel_size);
	}

	pthread_t *thread_handles = malloc(thread_count*sizeof(pthread_t));
	long thread;
	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, thread_func, (void*) thread); 
	}

	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL); 
	}

	if(HISTOGRAM_EQUALIZATION_MAPPING_DONE_ENABLED()){
		HISTOGRAM_EQUALIZATION_MAPPING_DONE(parallel_size);
	}

	return parallel_data;
}


/*
Exemplos:
./histogram_equalization <image_name> <program_version> <threads>
./histogram_equalization buffalo.pgm s       Executa o programa com a versão sequencial
./histogram_equalization buffalo.pgm p 8      Executa o programa com a versão paralela
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

		thread_count = atoi(argv[3]);
		printf("Threads: %d\n", thread_count);

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