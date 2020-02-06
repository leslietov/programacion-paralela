/*
 ============================================================================
 Name        : ejercicio6.c
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
void Print_vector(int b[], int n, char title[]);
 
int main(int argc, char *argv[]) {
	int n=8,rank, size, local_total=0,total=0,local_n,x;
	int A[n]={2,3,2,5,8,2,2,0};
	int *local_A;
	bool printResult=true;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(comm,&rank);
	MPI_Comm_size(comm,&size);
	if(rank==0) Print_vector(A,n,"Vector A");
	if(n%size==0){
		local_n=n/size;
		local_A=new int[local_n];
		MPI_Scatter(A,local_n,MPI_INT,local_A,local_n,MPI_INT,0,comm);
		if(rank==0){
				printf("Ingrese un número:");
				cin>>x;
		}
		MPI_Bcast(&x,1,MPI_INT,0,comm);
		for(int i=0;i<local_n;i++){
			if(local_A[i]==x){
				local_total++;
			}
		}
		MPI_Reduce(&local_total,&total,1,MPI_INT,MPI_SUM,0,comm);
	}else{
		printResult=false;
	}
	if(rank==0 ){
		if(!printResult){
			printf("número de procesos \"%d\" debe ser multiplo de la cantidad de elementos del arreglo\n",size);
		}else{
			printf("El número %d se repite %d veces\n",x,total);
		}
	}
	MPI::Finalize();
	return 0;
}
void Print_vector(
      int 	  b[]     /* in */,
      int     n       /* in */,
      char    title[] /* in */) {
   int i;
   printf("%s: [", title);
   for (i = 0; i < n; i++)
      printf("%d ", b[i]);
   printf("]\n");
}

