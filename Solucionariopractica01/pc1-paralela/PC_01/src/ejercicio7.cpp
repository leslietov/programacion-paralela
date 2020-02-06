/*
 ============================================================================
 Name        : ejercicio7.c
 Author      : FISI
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 
#include "mpi.h" 
#include <iostream>
using namespace std;
 
int main(int argc, char *argv[]) {
	int n, rank, size, local_fac=1,total_fac=1,local_n,u;
	bool printResult=true;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(comm,&rank);
	MPI_Comm_size(comm,&size);

	if(rank==0){
		printf("Ingrese un número:");
		cin>>n;
	}
	MPI_Bcast(&n,1,MPI_INT,0,comm);
	if(n==0|| n==1){
		total_fac=1;
	}else{
		if(n%size==0){
			local_n=n/size;
			for(int i=0;i<local_n;i++){
			   u=1+rank+i*size;
			 local_fac=local_fac*(u);
			}
			MPI_Reduce(&local_fac,&total_fac,1,MPI_INT,MPI_PROD,0,comm);
		}else{
			printResult=false;
		}
	}
	if(rank==0 ){
		if(!printResult){
			printf("El número ingresado \"%d\" no es divisible por el número de procesos  \"%d\"",n,size);
		}else{
			printf("El factorial de %d es %d",n,total_fac);
		}
	}
	MPI::Finalize();
	return 0;
}

