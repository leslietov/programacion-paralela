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
	int band;
	int local_exp;
	//Inicializar el programa
	MPI_Init(NULL,NULL);
	//Obtener rank y size
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Generar número del proceso
    local_num=rand()%20;
    if(local_num==0){
    	local_num=local_num+ 10*rank;
    	local_exp=0;
    }else{
    	local_num=local_num- 10*rank;
    	local_exp=1;
    }
    cout<<"El número del proceso "<<rank<<" es:"<<local_exp<<"\n";
    //Aplicar función de reducción
    MPI_Reduce(&local_exp,&band,1,MPI_INT,MPI_BAND,0,MPI_COMM_WORLD);
    if (rank == 0) {
    	//Imprimir resultado
	 cout<<"\nEl resultado del Band lógico es :"<<band<<"\n";
    }
	MPI_Finalize();
	return 0;
}
