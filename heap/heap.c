#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "heap.h"

#define DIM_INICIAL 10
#define FACTOR_REDIMENSION 2
#define FACTOR_CAPACIDAD 4

/* *****************************************************************
 *                DEFINICION DE ESTRUCTURAS
 * *****************************************************************/

struct heap {
	void** datos;
	size_t cant;
	size_t tam;
	cmp_func_t cmp;
};

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

void swap(void** arreglo, size_t x, size_t y) {
  void* aux = arreglo[x];
  arreglo[x] = arreglo[y];
  arreglo[y] = aux;
}

size_t maximo(void** arreglo, cmp_func_t cmp, size_t padre, size_t hijo_izq, size_t hijo_der, size_t cantidad) {
	size_t max = padre;
	if (hijo_izq < cantidad && cmp(arreglo[max], arreglo[hijo_izq]) < 0) max = hijo_izq;
	if (hijo_der < cantidad && cmp(arreglo[max], arreglo[hijo_der]) < 0) max = hijo_der;
	return max;
}

void heap_upheap(void** arreglo, size_t hijo, cmp_func_t cmp) {
	if (hijo == 0) return;

	size_t padre = (hijo-1) / 2;

	if (cmp(arreglo[padre], arreglo[hijo]) < 0) {
		swap(arreglo, padre, hijo);
		heap_upheap(arreglo, padre, cmp);
	}

}

void heap_downheap(void** arreglo, size_t cant, size_t padre, cmp_func_t cmp) {
	if (padre >= cant) return;

	size_t hijo_izq = (2 * padre) + 1;
	size_t hijo_der = (2 * padre) + 2;
	size_t max = maximo(arreglo, cmp, padre, hijo_izq, hijo_der, cant);

	if (max != padre) {
		swap(arreglo, padre, max);
		heap_downheap(arreglo, cant, max, cmp);
	}
}

bool heap_redimensionar(heap_t *heap, size_t tam_nuevo) {
	void **datos_nuevo = realloc(heap->datos, tam_nuevo * sizeof(void*));

	if (!datos_nuevo) return false;

	heap->datos = datos_nuevo;
	heap->tam = tam_nuevo;

	return true;
}

void heapify(void *elementos[], size_t cant, cmp_func_t cmp) {
	for (size_t i = cant; i != 0; i--) {
		heap_downheap(elementos, cant, i - 1, cmp);
	}
}

 /* *****************************************************************
 *                      PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));

	if (!heap) return NULL;

	heap->datos = malloc(DIM_INICIAL * sizeof(void*));

	if (!heap->datos) {
		free(heap);
		return NULL;
	}

	heap->cant = 0;
	heap->tam = DIM_INICIAL;
	heap->cmp = cmp;

	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	heap->datos = malloc(n * sizeof(void*));

	if (!heap->datos) {
		free(heap);
		return NULL;
	}

	for (size_t i = 0; i < n; i++) {
		heap->datos[i] = arreglo[i];
	}

	heapify(heap->datos, n, cmp);

	heap->cmp = cmp;
	heap->cant = n;
	heap->tam = n;

	return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
	if (!heap) return;

	if (destruir_elemento) {
		for (size_t i = 0; i < heap->cant; i++) {
			void* dato = heap->datos[i];
			destruir_elemento(dato);
		}
	}

	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
	return (heap->cant);
}

bool heap_esta_vacio(const heap_t *heap) {
	return (heap->cant == 0);
}

bool heap_encolar(heap_t *heap, void *elem) {
	if (heap->cant == heap->tam) {
		if (!heap_redimensionar(heap, heap->tam * FACTOR_REDIMENSION)) return false;
	}

	heap->datos[heap->cant] = elem;
	
	heap_upheap(heap->datos, heap->cant, heap->cmp);

	heap->cant++;

	return true;
}

void *heap_ver_max(const heap_t *heap) {
	if (!heap || (heap_esta_vacio(heap))) return NULL;
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap) {
	if (heap_esta_vacio(heap)) return NULL;

	void* dato = heap->datos[0];

	swap(heap->datos, 0, heap->cant-1);

	heap->cant--;

	heap_downheap(heap->datos, heap->cant, 0, heap->cmp);

	if (heap->tam > DIM_INICIAL && heap->tam >= heap->tam * FACTOR_CAPACIDAD) {
		heap_redimensionar(heap, heap->tam / FACTOR_REDIMENSION);
	}

	return dato;
}

 /* *****************************************************************
 *                      FUNCION HEAPSORT
 * *****************************************************************/

void _heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	if (cant == 0) return;

	swap(elementos, 0, cant-1);

	heap_downheap(elementos, cant-1, 0, cmp);
	
	_heap_sort(elementos, cant-1, cmp);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	if (cant == 0) return;

	heapify(elementos, cant, cmp);

	_heap_sort(elementos, cant, cmp);
}