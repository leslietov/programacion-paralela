/*
 ============================================================================
 Name        : Determinante_Paral.c
 Author      : FISI
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>

#define N 7

double determinantOfMatrix(double mat[N][N], int n);
double cofactor(double mat[N][N], int b,int j);
int main (int argc, char *argv[]) {
	double totalDet=0;
	srand(time(NULL));
  int tid;
  double mat[N][N];
  for(int i=0;i<N;i++){
  		for(int j=0;j<N;j++){
  			mat[i][j]=rand()%6;
  		}
  	}
  printf("\nMatriz generada:\n");
  for(int i=0;i<N;i++){
  		for(int j=0;j<N;j++){
  			printf("%lf ",mat[i][j]);
  		}
  		printf("\n");
  }
  	omp_set_num_threads(N);
	printf("\nResolviendo la determinate con %d hilos:\n",N);
  	#pragma omp parallel
 {
	 tid = omp_get_thread_num();
	 double cof=cofactor(mat,N,tid);
	 #pragma omp critical
	 totalDet+=cof;

 }
 printf("\nDeterminate de la matriz: %lf ",totalDet);
 return 0;
}

double determinantOfMatrix(double mat[N][N], int n)
{
	int index;
	double num1,num2,det = 1.0;
	double temp[n + 1];
	for(int i = 0; i < n; i++)
	{
		index = i;

		while(mat[index][i] == 0 && index < n) {
			index++;

		}
		if(index == n)
		{
			continue;

		}
		if(index != i)
		{
			for(int j = 0; j < n; j++)
			{
				int aux=mat[index][j];
				mat[index][j]=mat[i][j];
				mat[i][j]=aux;
			}
				if((index-i)%2!=0){
					det*=-1;
				}
	}

	for(int j = 0; j < n; j++)
	{
		temp[j] = mat[i][j];

	}
	for(int j = i+1; j < n; j++)
	{
		num1 = temp[i];
		num2 = mat[j][i];

		for(int k = 0; k < n; k++)
		{
			mat[j][k] = ((num1 * mat[j][k]) - (num2 * temp[k]))/num1;

		}
		}

	}

	for(int i = 0; i < n; i++)
	{
		det = det * mat[i][i];
	}

	return (det);
}

double cofactor(double mat[N][N], int b,int j){
	double menor[b][b];
	int c;
	for(int i=0;i<b;i++){
			for(int j=0;j<b;j++){
				menor[i][j]=0;
			}
		}

	for(int k=1; k<b; k++){

			c = 0;

			for(int l=0; l<b; l++){

					if(l!=j){

							menor[k-1][c] = mat[k][l];

							c++;

					}

			}

	}
	double determinant = mat[0][j]*determinantOfMatrix(menor,b-1);
	printf("Resultado por thread %d: %lf \n",j,determinant);
	if(j%2!=0)
		determinant*=-1;
	return determinant;
}
