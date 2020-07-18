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

#include <thread>
#include <mutex>
#include <vector>
#include "barrier.cpp"


double start, end;




void startTimer(){
	start = omp_get_wtime();
}

void stopTimer(){
	end = omp_get_wtime();
	double time = ((double) (end - start));
	printf("%f segundos\n", time);
}



unsigned char* sequential(unsigned char maxgray, int size, unsigned char* data){
	int* histogram = (int*) malloc(sizeof(int)*maxgray);
	int* normalized = (int*) malloc(sizeof(int)*maxgray);
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



static std::mutex m;




void* calculate_histogram(long thread_id, int size, int thread_count, int* histogram, unsigned char maxgray, unsigned char* data){	
	int* threadHistogram = (int*) malloc(maxgray*sizeof(int));
	memset(threadHistogram, 0, sizeof(int)*maxgray);

	int thread_chunck_size = size / thread_count;
	int start = thread_chunck_size*thread_id;
	int end = start + thread_chunck_size;

	if(thread_id == thread_count-1){
		end = size;
	}

	for(int i = start; i < end; i++) {
		threadHistogram[ data[i] ]++;
	}

	std::lock_guard<std::mutex> lock(m);
	for(int i=0; i <= maxgray; i++) {
		histogram[i] += threadHistogram[i];
	}

}


void* cumulative_distribution(long thread_id, int* histogram, int size, unsigned char maxgray,  int thread_count, int* cumulativeDistributionArray, int* normalized, Barrier& b){
	int cumulativeDistribution = 0;

	if(thread_id == 0){
		for(int i=0; i <= maxgray; i++) {
			cumulativeDistribution += histogram[i]; 
			cumulativeDistributionArray[i] = cumulativeDistribution;
		}
	}
	
	b.wait();


	int thread_chunck_size = maxgray/thread_count;
	int start = thread_chunck_size*thread_id;
	int end = start + thread_chunck_size;
	
	if(thread_id == thread_count-1){
		end = maxgray;
	}

	double constant = (double)maxgray / size;
	for(int i=start; i <= end; i++) {
		normalized[i] = round(cumulativeDistributionArray[i] * constant);
	}
}


void* map_new_values(long thread_id, int size, int thread_count, int* normalized, unsigned char* data){

	int thread_chunck_size = size / thread_count;
	int start = thread_chunck_size*thread_id;
	int end = start + thread_chunck_size;
	
	if(thread_id == thread_count-1){
		end = size;
	}
	
	for (int i = start; i < end; i++) {
		data[i] = normalized[data[i]];
	}

}


void* thread_func(long thread_id, int size, int thread_count, int* histogram, unsigned char maxgray, unsigned char* data, int* cumulativeDistributionArray, int* normalized, Barrier& b){

	calculate_histogram(thread_id, size, thread_count, histogram, maxgray, data);
	b.wait();


	if(thread_id == 0 && HISTOGRAM_EQUALIZATION_COMPUTING_DONE_ENABLED()){
		HISTOGRAM_EQUALIZATION_COMPUTING_DONE(size);
	}

	cumulative_distribution(thread_id, histogram, size, maxgray, thread_count, cumulativeDistributionArray, normalized, std::ref(b));
	b.wait();


	if(thread_id == 0 && HISTOGRAM_EQUALIZATION_NORMALIZING_DONE_ENABLED()){
		HISTOGRAM_EQUALIZATION_NORMALIZING_DONE(size);
	}

	map_new_values(thread_id, size, thread_count, normalized, data);
	b.wait();
}


unsigned char* parallel(int thread_count, unsigned char maxgray, int size, unsigned char* data){
    Barrier b(thread_count);

	int* histogram = (int*) malloc(maxgray*sizeof(int)); 
	memset(histogram, 0, sizeof(int)*maxgray);
	int* normalized = (int*) malloc(sizeof(int)*maxgray);
	memset(normalized, 0, sizeof(int)*maxgray);
	int* cumulativeDistributionArray = (int*) malloc(sizeof(int)*maxgray);

	if(HISTOGRAM_EQUALIZATION_EQUALIZATION_START_ENABLED()){
		HISTOGRAM_EQUALIZATION_EQUALIZATION_START(size);
	}


	std::vector<std::thread> threads;
	long thread;
	for (thread = 0; thread < thread_count; thread++) {
		std::thread t(thread_func, thread, size, thread_count, histogram, maxgray, data, cumulativeDistributionArray, normalized, std::ref(b));
		threads.push_back(std::move(t));
	}


	for(int i = 0; i < thread_count; i++){
		threads[i].join();
	}

	if(HISTOGRAM_EQUALIZATION_MAPPING_DONE_ENABLED()){
		HISTOGRAM_EQUALIZATION_MAPPING_DONE(size);
	}


	return data;
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

		int thread_count = atoi(argv[3]);

		printf("Threads: %d\n", thread_count);

		startTimer();
		newData = parallel(thread_count, maxgray, size, data);
		stopTimer();
	}
	else if(strcmp(argv[2], "io") == 0){
		printf("--------Only read and write--------\n");
	}
	
	char* output_file = (char*) "output.pgm";
	pgmb_write( output_file, xsize, ysize, newData, maxgray);

	return 0;
}