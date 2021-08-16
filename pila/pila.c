#include "pila.h"
#include <stdlib.h>

#define CANTIDAD_INICIAL 0
#define CAPACIDAD_INICIAL 5
#define MAX_REDIMENSION 4
#define REDIMENSION 2


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;   // Puntero a arreglo de punteros
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};


pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL) return NULL;

    pila->cantidad = CANTIDAD_INICIAL;
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->datos = malloc(pila->capacidad * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    return pila;
}


void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}


void wrapper_pila_destruir(void* pila){
    pila_destruir((pila_t*) pila);
}


bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == CANTIDAD_INICIAL;
}


bool pila_redimensionar(pila_t *pila, size_t cap_nueva){
    void **datos_nuevo = realloc(pila->datos, cap_nueva * sizeof(void*));

    if (datos_nuevo == NULL){
        return false;
    }

    pila->datos = datos_nuevo;
    pila->capacidad = cap_nueva;
    return true;
}


bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad){
        if (!(pila_redimensionar(pila, ((pila->capacidad)*REDIMENSION)))) return false;
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}


void *pila_ver_tope(const pila_t *pila){
    return (pila_esta_vacia(pila)) ? NULL : pila->datos[pila->cantidad-1];
}


void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)) return NULL;

    void* dato = pila->datos[(pila->cantidad)-1];

    if ((pila->cantidad)*MAX_REDIMENSION <= pila->capacidad) {
        pila_redimensionar(pila, ((pila->capacidad)/REDIMENSION));
    }
    
    pila->cantidad--;
    return dato;

}