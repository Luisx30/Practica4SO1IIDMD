//
// ParallelMatrix.c
// Practica4
//
// Created by Beatriz Paulina Garcia Salgado
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////No cambiar este segmento////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ParallelMatrix.h"

int getNumberOfCPUs()
/*Gets the number of available cores at the momment
Input: void
Output: number of available cores*/
{
	int numThreads;
	numThreads = omp_get_max_threads();
	return numThreads;
}

int checkSumAll(long double* counter, long double* M, int sizeMat)
/*Performs the addition of all the elements of a square matrix to check operations
Input:	pointer to save the result
	pointer to the matrix
	size of the square matrix
Output: EXIT_FAILURE; EXIT_SUCCESS*/
{
	int element, totalElem;
	if(!counter || !M)
	{
		printf("Error in input pointers\n");
		return EXIT_FAILURE;
	}

	totalElem = sizeMat * sizeMat;
	*counter = 0;
	for(element = 0; element < totalElem; element++)
	{
		*counter = *counter + *(M + element);
	}

	return EXIT_SUCCESS;
}


int ompParallelMatrixSum(long double** result, long double* A, long double* B, int sizeMat)
/*Computes the addition of two square matrices: result = A + B
Input:	pointer to save the result
	pointer to matrix A
	pointer to matrix B
	size of the square matrices
Output: EXIT_FAILURE; EXIT_SUCCESS*/
{
	int element, totalElem;

	if(!*result || !A || !B)
	{
		printf("Error in a matrix pointer\n");
		return EXIT_FAILURE;
	}

	totalElem = sizeMat * sizeMat;

	#pragma omp parallel for
	for(element = 0; element < totalElem; element++)
	{
		*(*result + element) = *(A + element) + *(B + element);
	}

	return EXIT_SUCCESS;
}

int matrixSum(long double** result, long double* A, long double* B, int sizeMat)
/*Computes the addition of two square matrices: result = A + B
Input:	pointer to save the result
	pointer to matrix A
	pointer to matrix B
	size of the square matrices
Output: EXIT_FAILURE; EXIT_SUCCESS*/
{
	int element, totalElem;

	if(!*result || !A || !B)
	{
		printf("Error in a matrix pointer\n");
		return EXIT_FAILURE;
	}

	totalElem = sizeMat * sizeMat;

	for(element = 0; element < totalElem; element++)
	{
		*(*result + element) = *(A + element) + *(B + element);
	}

	return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Inserte el código solicitado en la práctica aqui.
int Matrix_Mult(long double** result, long double* A, long double* B, int sizeMat){
	int element, totalElem, filapos, columnapos,i;
    long double fila[sizeMat], columna[sizeMat];
	if(!*result || !A || !B){
		printf("Error in a matrix pointer\n");
		return EXIT_FAILURE;
	}
	totalElem = sizeMat * sizeMat;
	for (element = 0; element < totalElem; element++){
		filapos = element / sizeMat;
		columnapos = element % sizeMat;
		*(*result + element) = 0;
		for (i = 0; i < sizeMat; i++){
			fila[i] = *(A + i + (sizeMat * filapos));
			columna[i] = *(B + (i * sizeMat) + columnapos);
			*(*result + element) += fila[i] * columna[i];
		}
	}
	return EXIT_SUCCESS;
}

int ompParallelMatrixMult(long double** result, long double* A, long double* B, int sizeMat){
    int element, totalElem, filapos, columnapos, chunk, i, numThreads;
    long double fila[sizeMat], columna[sizeMat];
	if(!*result || !A || !B){
		printf("Error in a matrix pointer\n");
		return EXIT_FAILURE;
	}
	totalElem = sizeMat * sizeMat;
	numThreads = getNumberOfCPUs();
	chunk = (int)round(sizeMat / numThreads);
	#pragma omp parallel for private(i, element, filapos, columnapos) schedule(dynamic, chunk)
	for (element = 0; element < totalElem; element++){
		filapos = element / sizeMat;
		columnapos = element % sizeMat;
		*(*result + element) = 0;
		#pragma omp parallel for
		for (i = 0; i < sizeMat; i++){
			fila[i] = *(A + i + (sizeMat * filapos));
			columna[i] = *(B + (i * sizeMat) + columnapos);
			*(*result + element) += fila[i] * columna[i];
		}
	}
	return EXIT_SUCCESS;
}

int Matrix_Max(long double* counter, long double* M, int sizeMat){
	int element, totalElem;
	if(!counter || !M){
		printf("Error in input pointers\n");
		return EXIT_FAILURE;
	}
	totalElem = sizeMat * sizeMat;
	*counter = 0;
	for(element = 0; element < totalElem; element++){
		if(*(M + element) > *counter){
            *counter = *( M + element );
        }
	}
	return EXIT_SUCCESS;
}

int ompParallelMatrixMax(long double* counter, long double* M, int sizeMat){
	int element, totalElem, chunk;
	int numThreads = getNumberOfCPUs();
	long double count = 0;
	if(!counter || !M){
		printf("Error in input pointers\n");
		return EXIT_FAILURE;
	}
	totalElem = sizeMat * sizeMat;
	chunk = (int)round(sizeMat / numThreads);
	#pragma omp parallel for shared(count) private(element) schedule(dynamic, chunk)
	for(element = 0; element < totalElem; element++){
        if(*(M + element) > count){
            count = *( M + element );
        }
	}
	*counter = count;
	return EXIT_SUCCESS;
}
