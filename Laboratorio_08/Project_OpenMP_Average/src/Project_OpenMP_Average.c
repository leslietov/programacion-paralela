/*
 ============================================================================
 Name        : Project_OpenMP_Average.c
 Author      : FISI
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
 * Hello OpenMP World prints the number of threads and the current thread id
 */
int main(int argc, char *argv[]) {

	int thread_count = strtol(argv[1], NULL, 10);

	int *fibseq = (int*) malloc(10 * sizeof(int));

	int suma = 0;

	srand(time(0));

	printf("[");

	for (int i = 0; i < 10; i++) {
		fibseq[i] = rand() % 10;

		printf("%d,", fibseq[i]);

	}

	printf("]");

	printf("\n");

	/* This creates a team of threads; each thread has own copy of variables  */
# pragma omp parallel num_threads(thread_count)
	{
		int tid = omp_get_thread_num();
		int numThreads = omp_get_num_threads();

		printf("Hello from thread number %d de un total %d \n", tid, numThreads);

		int local_n = 10 / numThreads;

		int local_a = tid * local_n;

		int local_b = local_a + local_n - 1;

		for (int i = local_a; i <= local_b; i++) {

			suma += fibseq[i];

		}

	}

	double average = (double) suma / 10;

	printf("EL promedio es %f", average);


		return 0;
	}

