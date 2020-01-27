/*
 ============================================================================
 Name        : SendMessage
 Author      : Herminio Paucar
 Version     :
 Copyright   : Your copyright notice
 Description : En este código de inicio se puede ver el funcionamiento de las
 funciones de envío y recepción. Lo que hace es que cada proceso
 se envía a sí mismo el mensaje deseado.
 Compile Command:
 $ mpiCC -g -Wall -o SendMessage01 SendMessage01.cpp
 $ mpiexec  ./SendMessage01
 $ mpiexec -n 10 ./SendMessage01
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "mpi.h"

const int MAX_STRING = 100;

int main(void) {
	char greeting[MAX_STRING];
	int comm_sz;
	int my_rank;
	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if (my_rank != 0) {
		sprintf(greeting, "Greetings from process !=0 %d of %d!",
		my_rank, comm_sz);

		MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0,
		MPI_COMM_WORLD);

	} else {
		printf("Greetings from process %d of %d!\n", my_rank, comm_sz);

		for (int q = 1; q < comm_sz; q++) {

			MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
			printf("%s\n", greeting);
		}
	}
	MPI_Finalize();
	return 0;
}

