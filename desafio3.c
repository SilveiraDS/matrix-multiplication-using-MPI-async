/**Trabalho Disciplina de IPPD - CMP134
 * 
 * Dieison Silveira
 * 
 * Produto de matrizes NxN com MPI ponto a ponto assíncrono (usar as operações MPI_Isend, MPI_Irecv, MPI_Test, MPI_Wait e variantes)
 * 
 * versao_3a
 * 
 * */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmp134.h"

#define tag 1
#define raiz 0

double *malloc_matrix(int, int);
void fill_matrix(double *, int, int);
void matrix_multiplication(double *, double *, double *, int, int, int, int);
void print_matrix(double *, int, int);
static unsigned int g_seed;
static inline int fastrand();

int main(int argc, char *argv[]) {
	double *A, *B, *C;
	int stripSize, rank, nodos, N, i, cont, numElements, offset, offsetB;
	CMP134_DECLARE_TIMER;

	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Request reqSend, reqRecv;
	MPI_Status statusRecv;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nodos);
	
	N = atoi(argv[1]);

/**quantidade de linhas que cada processo filho terá das matrizes A e B*/
	stripSize = N / nodos;
	numElements = N * stripSize;

/** O raiz tem as matrizes A,B e C com tamanho NxN */	
	if (rank == raiz) {
		A = malloc_matrix(N, N);
		B = malloc_matrix(N, N);
		C = malloc_matrix(N, N);
	} else {
		/** Os filhos tem somente a parte deles*/
		A = malloc_matrix(stripSize, N);
		B = malloc_matrix(stripSize, N);
		C = malloc_matrix(stripSize, stripSize); 
	}

/**inicializa as matrizes A e B com um valor aleatorio*/
	if (rank == raiz) {
		fill_matrix(A, N, N);
		fill_matrix(B, N, N);
	}

	CMP134_START_TIMER;

	/** Divide a matriz A em numElements partes e envia para filhos, raiz fica com sua parte*/
	if (rank == raiz) {
		for (i=1; i<nodos; i++) {
			MPI_Isend(&A[offset], numElements, MPI_DOUBLE, i, tag, comm, &reqSend); 
			offset += stripSize;
		}
	}
	else {
		MPI_Irecv(&A[0], numElements, MPI_DOUBLE, raiz, tag, comm, &reqRecv);
	}

	offsetB = 0;
	offset = numElements; //deslocamento inicial dos elementos do raiz
	for (cont = 0; cont < nodos; cont++){
		if (rank == raiz) {
			for (i=1; i<nodos; i++) {
				MPI_Isend(&B[offsetB], numElements, MPI_DOUBLE, i, tag, comm, &reqSend); 
			}
		}
		else {
			MPI_Irecv(&B[0], numElements, MPI_DOUBLE, raiz, tag, comm, &reqRecv);
			MPI_Wait(&reqRecv, &statusRecv);
		}

		/**calculo do produto de matriz*/
		matrix_multiplication(A, B, C, N, stripSize, rank, cont); //stripSize * stripSize elementos multiplicados por vez

		if (rank == raiz) {
			for (i=1; i<nodos; i++) {
				MPI_Irecv(&C[offset], stripSize * stripSize, MPI_DOUBLE, i, tag, comm, &reqRecv);
				MPI_Wait(&reqRecv, &statusRecv);
				offset += stripSize * stripSize;
			}
		}else{
			MPI_Isend(&C[0], stripSize * stripSize, MPI_DOUBLE, raiz, tag, comm, &reqSend);
		}

		offsetB += stripSize; //atualiza offsetB para proxima iteracao
	}

//**stop timer
	CMP134_END_TIMER;
	CMP134_REPORT_TIMER;

	// if (rank == 0) {print_matrix(C, N, N);}

	MPI_Finalize();

	return 0;
}

/** Funções */
static inline int fastrand(){
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}

void matrix_multiplication(double *A, double *B, double *C, int N, int stripSize, int rank, int p){

	int i, j, k;
	double temp;
	int position = 0;
	
	/** corrigi a posição inicial para o raiz*/
	if (rank == 0) {position = p * stripSize*stripSize;}

	for (i=0; i<stripSize; i++) { 
		for (j=0; j<stripSize; j++) {
			temp = 0;
			for (k=0; k<N; k++) {
				temp = temp + (A[i*N+k] * B[j*N+k]);
			}
			C[position] = temp;
			position++;
		}
	}
}

double *malloc_matrix(int l, int c) {
 	double *matrix;
    if ((matrix = malloc(l * c * sizeof (double*))) == NULL) {
        fprintf(stderr, "Nao foi possivel alocar memoria!\n");
        exit(1);
    }
    return matrix;
}

void fill_matrix(double *matrix, int l, int c){
	int i;
	for (i=0; i<l*c; i++) {
		matrix[i] = (double) fastrand();
		// matrix[i] = 2; // for test
	}
}

void print_matrix(double *matrix, int l, int c){
	int i, j;
	for (i=0; i<l; i++) {
		for (j=0; j<c; j++) {
			printf("%.2f ",matrix[i*c+j]);
		}
		printf("\n");
	}
}