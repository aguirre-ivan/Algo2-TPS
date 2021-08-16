#include "tp0.h"
#include <stdio.h>
#include <stdlib.h>

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
	int aux = *x;
	*x = *y;
	*y = aux;
}


int maximo(int vector[], int n) {
	if (n == 0) {
		return -1;
	}
	int max = 0;
	for (int i = 0; i < n; i++) {
		if (vector[i] > vector[max]) {
			max = i;
		}
	}
	return max;
}


int menor(int n1, int n2){
	if (n1 < n2) {
		return n1;
	}
	return n2;
}

int comparar(int vector1[], int n1, int vector2[], int n2) {
	int n = menor(n1, n2);
	for (int i = 0; i < n; i++) {
		if (vector1[i] == vector2[i]) {
			continue;
		}
		if (vector1[i] > vector2[i]) {
			return 1;
		}
		return -1;
	}
	if (n1 == n2) {
		return 0;
	}
	if (n1 == n) {
		return -1;
	}
	return 1;
}


void seleccion(int vector[], int n) {
	int pos_maximo;
	for (int i = n-1; i > 0; i--) {
		pos_maximo = maximo(vector, i+1);
		swap(&vector[i], &vector[pos_maximo]);
	}
}
