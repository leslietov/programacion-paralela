/*
 ============================================================================
 Name        : Proyecto_MpiOperators.c
 Author      : Leslie Tovar
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 
#include <stdlib.h>
#include <ctime>
#include "mpi.h" 
#include <iostream>
using namespace std;



int main(int argc, char *argv[]) {
	int rank, size;
	double local_num;
	double prod;
	//Inicializar el programa
	MPI_Init(NULL,NULL);
	//Obtener rank y size
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Generar número del proceso
    local_num=rand()%20;


    printf("El número del proceso %i es: %lf\n",rank,local_num);
    //Aplicar función de reducción
    MPI_Reduce(&local_num,&prod,1,MPI_DOUBLE,MPI_PROD,0,MPI_COMM_WORLD);
    if (rank == 0) {
    	//Imprimir resultado
	  printf("\nEL producto de los números generados es : %lf \n", prod);
    }
	MPI_Finalize();
	return 0;
}

