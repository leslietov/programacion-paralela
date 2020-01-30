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
	int ID;
	int data=0;
	int min=0;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &ID);

	data=(ID+1)*100;

	if (ID== 0) {
	      cout<<"Antes MPI_Reduce\n";
	      cout<<"min ess"<<min<< endl;

	    }
 //Aplicar función de reducción
    MPI_Reduce(&data,&min,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
    if (ID== 0) {
      cout<<"Despues MPi_Reduce\n";
      cout<<"min es"<<min<< endl;
    }
	MPI_Finalize();
	return 0;
}

