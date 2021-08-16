#include "lista.h"
#include <stdlib.h>

// ESTRUCUTRAS

typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;

struct lista {
    struct nodo* prim;
    struct nodo* ult;
    size_t largo;
};

struct lista_iter {
    lista_t* lista;
    nodo_t* anterior;
    nodo_t* actual;
};

nodo_t* nodo_crear(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (!nodo) return NULL;

    nodo->dato = valor;
    nodo->prox = NULL;

    return nodo;
}


// PRIMITIVAS LISTA

lista_t *lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));

    if (lista == NULL) return NULL;

    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nuevo_nodo = nodo_crear(dato);
    if (!nuevo_nodo) return false;

    if (lista_esta_vacia(lista)) {
        lista->ult = nuevo_nodo;
    } else {
        nuevo_nodo->prox = lista->prim;
    }
    lista->prim = nuevo_nodo;
    lista->largo++;

    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t* nuevo_nodo = nodo_crear(dato);
    if (!nuevo_nodo) return false;

    if (lista_esta_vacia(lista)) {
        lista->prim = nuevo_nodo;
    } else {
        lista->ult->prox = nuevo_nodo;
    }
    lista->ult = nuevo_nodo;
    lista->largo++;

    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;

    void* dato = lista->prim->dato;
    nodo_t* nodo_borrar = lista->prim;

    lista->prim = lista->prim->prox;
    if (lista->largo == 1) lista->ult = NULL;
    lista->largo--;

    free(nodo_borrar);
    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;
    return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) return NULL;
    return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato) destruir_dato(dato);
    }
    free(lista);
}


// PRIMITIVA ITERADOR INTERNO

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    if (visitar == NULL || lista_esta_vacia(lista)) return;

    nodo_t* actual = lista->prim;

    while (actual) {
        if (!visitar(actual->dato, extra)) break;
        actual = actual->prox;
    }
}


// PRIMITIVAS ITERADOR EXTERNO

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
    if (!iterador) return NULL;

    iterador->lista = lista;
    iterador->actual = lista->prim;
    iterador->anterior = NULL;

    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return false;
    
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox; 

    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;

    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    if (!iter->anterior) {
        if (lista_insertar_primero(iter->lista, dato)) {
            iter->actual = iter->lista->prim;
            return true;
        }
        return false;
    }

    if (lista_iter_al_final(iter)) {
        if (lista_insertar_ultimo(iter->lista, dato)) {
            iter->actual = iter->lista->ult;
            return true;
        }
        return false;
    }

    nodo_t* nodo_nuevo = nodo_crear(dato);
    if (!nodo_nuevo) return false;

    iter->anterior->prox = nodo_nuevo;
    nodo_nuevo->prox = iter->actual;
    iter->actual = nodo_nuevo;
    iter->lista->largo++;

    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;

    if (iter->anterior == NULL) {
        iter->actual = iter->lista->prim->prox;
        return lista_borrar_primero(iter->lista);
    }

    if (iter->actual->prox == NULL) iter->lista->ult = iter->anterior;

    nodo_t* nodo_borrar = iter->actual;
    void* dato = iter->actual->dato;
    iter->anterior->prox = iter->actual->prox;
    iter->actual = iter->actual->prox;
    free(nodo_borrar);
    iter->lista->largo--;
    return dato;
}
