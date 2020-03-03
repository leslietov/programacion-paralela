/*
 ============================================================================
 Name        : Mochila_Parall.c
 Author      : FISI
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */

#include <omp.h>
#include <stdio.h>

int max(int a, int b) {
	return (a > b) ? a : b;
}

int knapSack(int W, int wt[], int val[], int n) {
	int i, w;
	int K[n + 1][W + 1];

	for (i = 0; i <= n; i++) {
		for (w = 0; w <= W; w++) {
			if (i == 0 || w == 0)
				K[i][w] = 0;
			else if (wt[i - 1] <= w) {
				K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
			} else
				K[i][w] = K[i - 1][w];
		}
	}

	return K[n][W];
}

int main(int argc, char *argv[]) {

	int val[] = { 60, 100, 200, 50};
	int wt[] = { 10, 20, 30, 20 };
	int W = 50;
	int n = sizeof(val) / sizeof(val[0]);
	int resultado = 0;

	#pragma omp parallel num_threads(2)
	{
		int id = omp_get_thread_num();
		int nt = omp_get_num_threads();
		int size = (int) n / nt;
		int ini = id * size;
		int fin = ini + size - 1;

		int wt_hilo[size];
		int val_hilo[size];
		int contador = 0;

		for (int i = ini; i <= fin; i++) {
			wt_hilo[contador] = wt[i];
			val_hilo[contador] = val[i];
			contador++;
		}

		for (int j = ini; j <= fin; j++) {
			resultado = knapSack(W, wt_hilo, val_hilo, size);
		}

		printf("%d\n", resultado);

	}

	return 0;
}
