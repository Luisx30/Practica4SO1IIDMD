//
// Main.c
// Practica4
//
// Created by Beatriz Paulina Garcia Salgado
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ParallelMatrix.h"

int main(int argc, char** argv)
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////No cambiar este segmento////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Variables for matrix operations
	long double* A = NULL;
	long double* B = NULL;
	long double* C = NULL;
	long double count;
	int sizeMat = 100;
	int row,col;

	//Variables for thread management and time
	int numThreads;
	struct timespec t0, t1;
	double tiempoMedido;

	//Allocate matrices
	A = (long double*)malloc(sizeMat * sizeMat * sizeof(long double));
	B = (long double*)malloc(sizeMat * sizeMat * sizeof(long double));
	C = (long double*)malloc(sizeMat * sizeMat * sizeof(long double));
	if(!A || !B || !C)
	{
		printf("Cannot allocate the matrices\n");
		exit(EXIT_FAILURE);
	}

	//Set the maximum number of cores as available threads
	numThreads = getNumberOfCPUs();
	printf("Available cores: %d\n",numThreads);
	omp_set_num_threads(numThreads);

	//Initialise the matrices
	for(row = 0; row < sizeMat; row++)
	{
		count = 1;
		for(col = 0; col < sizeMat; col++)
		{
			*(A + row * sizeMat + col) = count;
			*(B + row * sizeMat + col) = 2;
			count++;
		}
	}

	checkSumAll(&count, A, sizeMat);
	printf("The addition of all components of A is %Lf\n",count);

	checkSumAll(&count, B, sizeMat);
	printf("The addition of all components of B is %Lf\n",count);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//La multiplicacion a realizar es A*B.
	//El maximo debe obtenerse de la matriz A.

	//Inserte el codigo solicitado en la practica aqui.

	///A. Función de multiplicación no paralela
    if (clock_gettime(CLOCK_MONOTONIC, &t0) != 0){
		printf("Error al llamar clock_gettime para t0\n");
		exit(EXIT_FAILURE);
	}

	Matrix_Mult(&C, A, sizeMat);

	if (clock_gettime(CLOCK_MONOTONIC, &t1) != 0){
		printf("Error al llamar clock_gettime para t1\n");
		exit(EXIT_FAILURE);
	}

	tiempoMedido = (double)(t1.tv_sec - t0.tv_sec) + ((double)(t1.tv_nsec - t0.tv_nsec) / 1000000000L);
	float tiempomultinoparalelo = tiempoMedido;

	checkSumAll(&count, C, sizeMat);

    printf("Funcion Multiplicacion no Paralela");
    printf("\nThe addition of all components of the product A x B is %Lf",count);
	printf("\nTiempo del proceso en Multiplicacion No paralela: %lf seconds", tiempoMedido);
    printf("\n\n");

	///B. Función de multiplicación paralela
	if (clock_gettime(CLOCK_MONOTONIC, &t0) != 0)
	{
		printf("Error al llamar clock_gettime para t0\n");
		exit(EXIT_FAILURE);
	}

	ompParallelMatrixMult(&C, A, B, sizeMat);

	if (clock_gettime(CLOCK_MONOTONIC, &t1) != 0)
	{
		printf("Error al llamar clock_gettime para t1\n");
		exit(EXIT_FAILURE);
	}

	tiempoMedido = (double)(t1.tv_sec - t0.tv_sec) + ((double)(t1.tv_nsec - t0.tv_nsec) / 1000000000L);
	float tiempomultiparalelo= tiempoMedido;

	checkSumAll(&count, C, sizeMat);

    printf("Funcion Multiplicacion Paralela");
	printf("\nThe addition of all components of the product A x B is %Lf",count);
    printf("\nTiempo del proceso para multiplicacion Paralela: %lf seconds", tiempoMedido);
    printf("\n\n");

	///F. Función de máximo no paralela
	if (clock_gettime(CLOCK_MONOTONIC, &t0) != 0){
		printf("Error al llamar clock_gettime para t0\n");
		exit(EXIT_FAILURE);
	}

	Matrix_Max(&count, A, sizeMat);

	if (clock_gettime(CLOCK_MONOTONIC, &t1) != 0){
		printf("Error al llamar clock_gettime para t1\n");
		exit(EXIT_FAILURE);
	}

	tiempoMedido = (double)(t1.tv_sec - t0.tv_sec) + ((double)(t1.tv_nsec - t0.tv_nsec) / 1000000000L);
	float tiempomaxnoparalelo = tiempoMedido;

	printf("Funcion Maximo no Paralela");
	printf("\nThe max value in  A  is %Lf",count);
	printf("\nTiempo del proceso para maximo No paralela: %lf seconds", tiempoMedido);
	printf("\n\n");

	///G. Función de máximo paralela
	if (clock_gettime(CLOCK_MONOTONIC, &t0) != 0){
		printf("Error al llamar clock_gettime para t0\n");
		exit(EXIT_FAILURE);
	}

	ompParallelMatrixMax(&count, A, sizeMat);

	if (clock_gettime(CLOCK_MONOTONIC, &t1) != 0){
		printf("Error al llamar clock_gettime para t1\n");
		exit(EXIT_FAILURE);
	}

	tiempoMedido = (double)(t1.tv_sec - t0.tv_sec) + ((double)(t1.tv_nsec - t0.tv_nsec) / 1000000000L);
	float tiempomaxparalelo = tiempoMedido;

    printf("Funcion Maximo Paralela");
    printf("\nThe max value in  A  is %Lf",count);
	printf("\nTiempo del proceso para maximo paralelo: %lf seconds", tiempoMedido);
	printf("\n\n");


	///J. Presentación en consola de una tabla comparativa de tiempos de ejecución
	printf("_______________________________________________________\n");
	printf("|                 |    No paralelo   |    Paralelo     |\n");
	printf("_______________________________________________________\n");
	printf("| Maximo          |     %f           |     %f          |\n",tiempomaxnoparalelo, tiempomaxparalelo);
	printf("| Multiplicacion  |     %f           |     %f          |\n",tiempomultinoparalelo, tiempomultiparalelo);
	printf("_______________________________________________________\n");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return EXIT_SUCCESS;
}
