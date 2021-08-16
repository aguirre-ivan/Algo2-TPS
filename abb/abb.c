#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* *****************************************************************
 *                DEFINICION DE ESTRUCTURAS
 * *****************************************************************/

typedef struct abb_nodo {
    struct abb_nodo *izq;
    struct abb_nodo *der;
    char* clave;
    void* dato;
} nodo_t;

struct abb {
    nodo_t* raiz;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t destruir_dato;
    size_t cantidad;
};

struct abb_iter {
    pila_t* pila_iter;
};

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_t* crear_nodo(const char* clave, void* dato) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) return NULL;

    char* clave_dup = strdup(clave);
    if (!clave_dup) {
        free(nodo);
        return NULL;
    }

    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->clave = clave_dup;
    nodo->dato = dato;

    return nodo;
}

// Busca la clave y devuelve el nodo donde se encuentra o NULL
// en caso de que no este, guarda la referencia a su padre
nodo_t* buscar_clave(abb_comparar_clave_t comparar, const char* clave, nodo_t* nodo_actual, nodo_t** p_nodo_padre) {
    if (!nodo_actual) return NULL;

    int comparacion = comparar(nodo_actual->clave, clave);

    if (comparacion == 0) return nodo_actual;
    
    *p_nodo_padre = nodo_actual;

    if (comparacion > 0) {
        return buscar_clave(comparar, clave, nodo_actual->izq, p_nodo_padre);
    }

    return buscar_clave(comparar, clave, nodo_actual->der, p_nodo_padre);
}

void _destruir_dato(nodo_t* nodo, abb_destruir_dato_t destruir_dato) {
    if (destruir_dato) destruir_dato(nodo->dato);
}

void destruir_nodos_recursivamente(nodo_t* nodo, abb_destruir_dato_t destruir_dato) {
    if (!nodo) return;

    destruir_nodos_recursivamente(nodo->izq, destruir_dato);
    destruir_nodos_recursivamente(nodo->der, destruir_dato);

    _destruir_dato(nodo, destruir_dato);
    free(nodo->clave);
    free(nodo);
}

// Busca un reemplazante para el nodo_actual, y guarda la 
// referencia a su padre
// Un reemplazante es el elemento "mas a la derecha" (elem mayor)
// de su subarbol izquierdo.
nodo_t* buscar_reemplazante(nodo_t* nodo_actual, nodo_t** p_nodo_padre) {
    nodo_t* nodo_reemplazante = nodo_actual->izq;

    while (nodo_reemplazante->der) {
        *p_nodo_padre = nodo_reemplazante;
        nodo_reemplazante = nodo_reemplazante->der;
    }

    return nodo_reemplazante;
}

// Borra la referencia al nodo de su padre.
// Si padre es NULL no hace nada
// NO libera la memoria del nodo borrado
// Pre: el parametro verificar_raiz debe ser true si se llama la funcion para borrar 1 o 0 hijos, 
// false en caso de 2 hijos, ya que no se borra el nodo sino que se reemplaza la calve y dato.
void borrar_referencia_a_nodo(abb_t* arbol, nodo_t* nodo_borrar, nodo_t* nodo_padre, bool verificar_raiz) {
    nodo_t* nuevo_hijo = nodo_borrar->izq ? nodo_borrar->izq : nodo_borrar->der; // Si ambos son NULL, no tenia hijos entonces el nuevo hijo de nodo_padre es NULL

    if (nodo_padre == NULL && verificar_raiz) arbol->raiz = nuevo_hijo;

    if (!nodo_padre) return;

    if (nodo_padre->der && arbol->comparar(nodo_padre->der->clave, nodo_borrar->clave) == 0) {
        nodo_padre->der = nuevo_hijo;
    } else {
        nodo_padre->izq = nuevo_hijo;
    }
}

// En la pila del iter, apila el nodo recibido y todos sus nodos izquierdos
bool apilar_izquierdos(abb_iter_t* iter, nodo_t* nodo) {
    if (!nodo) return true;

    if (!pila_apilar(iter->pila_iter, (void*) nodo)) return false;

    return apilar_izquierdos(iter, nodo->izq);
}

/* *****************************************************************
 *                      PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t comparar, abb_destruir_dato_t destruir_dato) {
    abb_t* arbol = malloc(sizeof(abb_t));
    if (arbol == NULL) return NULL;

    arbol->raiz = NULL;
    arbol->comparar = comparar;
    arbol->destruir_dato = destruir_dato; 
    arbol->cantidad = 0;

    return arbol;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato) {
    nodo_t* nodo_padre = NULL;
    nodo_t* nodo_actual = buscar_clave(arbol->comparar, clave, arbol->raiz, &nodo_padre);

    if (abb_pertenece(arbol, clave)) {

        _destruir_dato(nodo_actual, arbol->destruir_dato);
        nodo_actual->dato = dato;
        return true;

    } else {
        
        nodo_t* nodo_nuevo = crear_nodo(clave, dato);
        if (!nodo_nuevo) return false;

        abb_comparar_clave_t comparar = arbol->comparar;

        if (arbol->cantidad == 0) {
            arbol->raiz = nodo_nuevo;
        } else {

            if (comparar(nodo_padre->clave, clave) > 0) {
                nodo_padre->izq = nodo_nuevo;
            } else {
                nodo_padre->der = nodo_nuevo;
            }
        }

        arbol->cantidad++;
    }

    return true;
}

void* abb_borrar(abb_t* arbol, const char* clave) {
    if (!abb_pertenece(arbol, clave)) return NULL;

    nodo_t* padre_borrar = NULL;
    nodo_t* nodo_borrar = buscar_clave(arbol->comparar, clave, arbol->raiz, &padre_borrar);
    void* dato_res = nodo_borrar->dato;

    if (nodo_borrar->izq && nodo_borrar->der) { // borrar con 2 hijos
        nodo_t* padre_reemplazante = nodo_borrar;
        nodo_t* nodo_reemplazante = buscar_reemplazante(nodo_borrar, &padre_reemplazante);

        // En este caso no borramos el nodo
        char* clave_reemplazante = nodo_reemplazante->clave;
        void* dato_reemplazante = nodo_reemplazante->dato;
        free(nodo_borrar->clave);
        nodo_borrar->clave = clave_reemplazante;
        nodo_borrar->dato = dato_reemplazante;

        borrar_referencia_a_nodo(arbol, nodo_reemplazante, padre_reemplazante, false); // false porque si "borramos" la raiz, el nodo sigue siendo el mismo
        free(nodo_reemplazante);
        
    } else { // borrar con 0 o 1 hijo
        borrar_referencia_a_nodo(arbol, nodo_borrar, padre_borrar, true); // podemos llegar a borrar la raiz

        free(nodo_borrar->clave);
        free(nodo_borrar);
    }

    arbol->cantidad--;
    return dato_res;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    if (!abb_pertenece(arbol, clave)) return NULL;
    nodo_t* nodo_padre = NULL;
    nodo_t* busqueda = buscar_clave(arbol->comparar, clave, arbol->raiz, &nodo_padre);
    return busqueda->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    nodo_t* nodo_padre = NULL;
    return buscar_clave(arbol->comparar, clave, arbol->raiz, &nodo_padre) != NULL;
}

size_t abb_cantidad(const abb_t *arbol) {
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
    destruir_nodos_recursivamente(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

bool _abb_in_order(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
    if (!nodo) return true;

    if (!_abb_in_order(nodo->izq, visitar, extra)) return false;
    if (visitar && !visitar(nodo->clave, nodo->dato, extra)) return false;
    if (!_abb_in_order(nodo->der, visitar, extra)) return false;

    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    if (arbol->cantidad == 0) return;

    _abb_in_order(arbol->raiz, visitar, extra);
}

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t* abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* iter = malloc(sizeof(iter));
    if (!iter) return NULL;

    pila_t* pila = pila_crear();
    if (!pila) {
        free(iter);
        return NULL;
    }

    iter->pila_iter = pila;

    if (!apilar_izquierdos(iter, arbol->raiz)) {
        pila_destruir(iter->pila_iter);
        free(iter);
        return NULL;
    }

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t* iter) {
    if (abb_iter_in_al_final(iter)) return false;

    nodo_t* nodo_desapilado = (nodo_t*) pila_desapilar(iter->pila_iter);

    if (nodo_desapilado->der) apilar_izquierdos(iter, nodo_desapilado->der);

    return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter) {
    if (abb_iter_in_al_final(iter)) return NULL;
    nodo_t* tope = (nodo_t*) pila_ver_tope(iter->pila_iter);
    return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t* iter) {
    return pila_esta_vacia(iter->pila_iter);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila_iter);
    free(iter);
}