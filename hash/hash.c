#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define DIMENSION_INICIAL 97
#define INDICE_NULO -1

#define MIN_FACTOR_CARGA 0.1
#define MAX_FACTOR_CARGA 0.7
#define FACTOR_AGRANDAR 3
#define FACTOR_ACHICAR 2

/* *****************************************************************
 *                DEFINICION DE ESTRUCTURAS
 * *****************************************************************/
typedef enum estado {
    BORRADO,
    OCUPADO,
    VACIO,
} estado_posicion_t;

typedef struct hash_campo {
    char* clave;
    void* dato;
    estado_posicion_t estado;
} hash_campo_t;

struct hash {
    size_t dimension;                   // dimension de la tabla interna de hash
    size_t cantidad_elementos;             // cantidad de claves guardadas
    size_t cantidad_borrados;             // cantidad de lugares borrados
    hash_campo_t* tabla_campos;              // arreglo de campos
    hash_destruir_dato_t destruir_dato; // funcion recibida al crear hash
};

struct hash_iter {
    const hash_t* hash;
    size_t indice_actual;
};

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

// Funcion de hashing (sbdm) -> http://profesores.elo.utfsm.cl/~agv/elo320/miscellaneous/hashFunction/hashFunction.html
static unsigned long sdbm(unsigned char *str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// Wrapper de la funcion de hashing sdbm para castear size_t
size_t funcion_hashing(unsigned char *str) {
    return (size_t) (sdbm(str));
}

// Booleano para verificar igualdad de claves
bool claves_iguales(const char* clave1, const char* clave2) {
    if (clave1 == NULL || clave2 == NULL) return false;
    return strcmp(clave1, clave2) == 0;
}

// Consigue el indice posible donde deberia encontrarse la clave.
size_t hash_indice_posible(const hash_t* hash, const char* clave) {
    size_t indice = funcion_hashing((unsigned char*) clave) % hash->dimension;

    while (hash->tabla_campos[indice].estado != VACIO) {
        if (hash->tabla_campos[indice].estado == OCUPADO) {
            if (claves_iguales(hash->tabla_campos[indice].clave, clave)) return indice;
        }
    
        indice++;

        if (indice == hash->dimension) indice = 0;
    }

    return indice;
}

// Consigue el indice donde se encuentra la clave, en caso de no
// existir, devuelve INDICE_NULO
int hash_indice_clave(const hash_t* hash, const char* clave) {
    int indice = (int) hash_indice_posible(hash, clave);
    
    return claves_iguales(hash->tabla_campos[indice].clave, clave) ? indice : INDICE_NULO;
}

// Las funciones verifican los factores de carga
bool factor_maximo_correcto(hash_t* hash) {
    double factor_carga = (double) (hash->cantidad_elementos + hash->cantidad_borrados + 1) / (double) hash->dimension; //+1 ya que se verifica antes de agregar el elemento
    return factor_carga < MAX_FACTOR_CARGA;
}

bool factor_minimo_correcto(hash_t* hash) {
    double factor_carga_min = (double) hash->cantidad_elementos / (double) hash->dimension; //factor sin contemplar borrados
    return MIN_FACTOR_CARGA < factor_carga_min;
}

bool hay_que_achicar(hash_t* hash) {
    if (hash->dimension <= DIMENSION_INICIAL) return false;
    return !factor_minimo_correcto(hash);
}

// Asigna en NULL la clave y el dato, en el estado el estado recibido
// por parametro
void hash_asignar_campo(hash_campo_t* hash_campo, estado_posicion_t estado) {
    hash_campo->clave = NULL;
    hash_campo->dato = NULL;
    hash_campo->estado = estado;
}

// Se asigna estado BORRADO, y todos los punteros se inicializan en 0
// Se pierde la referencia al dato, por lo que hay que tener precaucion antes de utilizarla
void hash_borrar_campo(hash_campo_t* hash_campo) {
    free(hash_campo->clave);
    hash_asignar_campo(hash_campo, BORRADO);
}

void hash_vaciar_campo(hash_campo_t* hash_campo) {
    hash_asignar_campo(hash_campo, VACIO);
}

// Inicializa los campos del arreglo a vacios
void inicializar_campos_vacios(hash_campo_t* hash_campo, size_t dimension) {
    for (int i = 0; i < dimension; i++) { 
        hash_vaciar_campo(hash_campo + i);
    }
}

void hash_destruir_dato(hash_campo_t* hash_campo, hash_destruir_dato_t destruir_dato) {
    if (destruir_dato != NULL) {
        destruir_dato(hash_campo->dato);
    }
    hash_campo->dato = NULL;
}

// Redimensiona el hash y devuelve un booleano que indica si se
// pudo redimensionar o no.
bool hash_redimensionar(hash_t* hash, size_t nueva_dimension) {

    hash_campo_t* nueva_tabla = malloc(sizeof(hash_campo_t) * nueva_dimension);
    if (nueva_tabla == NULL) return false;
    inicializar_campos_vacios(nueva_tabla, nueva_dimension);

    hash_campo_t* tabla_borrar = hash->tabla_campos;
    size_t dimension_ant = hash->dimension;
    hash->dimension = nueva_dimension;
    hash->tabla_campos = nueva_tabla;
    hash->cantidad_borrados = 0;
    hash->cantidad_elementos = 0;

    for (int i = 0; i < dimension_ant; i++) {
        if (tabla_borrar[i].estado != OCUPADO) continue;
        hash_guardar(hash, tabla_borrar[i].clave, tabla_borrar[i].dato);
        hash_borrar_campo(tabla_borrar + i);
    }

    free(tabla_borrar);

    return true;
}

/* *****************************************************************
 *                        PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) return NULL;

    hash->dimension = DIMENSION_INICIAL;
    hash->cantidad_elementos = 0;
    hash->cantidad_borrados = 0;
    hash->destruir_dato = destruir_dato;

    hash_campo_t* tabla_campos = malloc(sizeof(hash_campo_t) * DIMENSION_INICIAL);

    if (tabla_campos == NULL) {
        free(hash);
        return NULL;
    }

    inicializar_campos_vacios(tabla_campos, hash->dimension);
    hash->tabla_campos = tabla_campos;

    return hash;
}

bool hash_guardar(hash_t *hash, const char* clave, void* dato) {

    if (!factor_maximo_correcto(hash)) {
        
        if (!hash_redimensionar(hash, hash->dimension * FACTOR_AGRANDAR)) return false;
    }


    size_t indice = hash_indice_posible(hash, clave);
    bool aumentar_cantidad = true;

    if (hash_pertenece(hash, clave)) {
        hash_destruir_dato(hash->tabla_campos + indice, hash->destruir_dato);
        aumentar_cantidad = false; // La clave esta contemplada en la cantidad
    } else {
        char* clave_duplicada = strdup(clave);
        if (!clave_duplicada) return false;

        hash->tabla_campos[indice].estado = OCUPADO;
        hash->tabla_campos[indice].clave = clave_duplicada;
    }

    hash->tabla_campos[indice].dato = dato;

    if (aumentar_cantidad) hash->cantidad_elementos++;

    return true;
}

void* hash_borrar(hash_t* hash, const char* clave) {

    if (!hash_pertenece(hash, clave)) return NULL;

    void* dato = hash_obtener(hash, clave);
    size_t indice = (size_t) hash_indice_clave(hash, clave);

    hash_borrar_campo(hash->tabla_campos + indice);
    hash->cantidad_borrados++;
    hash->cantidad_elementos--;
    
    if (hay_que_achicar(hash)) {
        hash_redimensionar(hash, hash->dimension / FACTOR_ACHICAR);
    }
    
    return dato;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
    
    if (!hash_pertenece(hash, clave)) return NULL;

    size_t indice = (size_t) hash_indice_clave(hash, clave);

    return hash->tabla_campos[indice].dato;
}

bool hash_pertenece(const hash_t* hash, const char* clave){
    if (clave == NULL) return false; // Por implementacion de iterador
    return hash_indice_clave(hash, clave) != INDICE_NULO;
}

size_t hash_cantidad(const hash_t* hash) {
    return hash->cantidad_elementos;
}

void hash_destruir(hash_t* hash) {
    for (int i = 0; i < hash->dimension; i++) {
        if (hash->tabla_campos[i].estado != OCUPADO) continue;
        hash_destruir_dato(hash->tabla_campos + i, hash->destruir_dato);
        hash_borrar_campo(hash->tabla_campos + i);
    }
    free(hash->tabla_campos);
    free(hash);
}

/* *****************************************************************
 *                      PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t* hash_iter_crear(const hash_t* hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (iter == NULL) return NULL;

    iter->hash = hash;

    if (hash->cantidad_elementos == 0) {
        iter->indice_actual = hash->dimension;
    } else {
        size_t indice = 0;
        while (hash->tabla_campos[indice].estado != OCUPADO) indice++;
        iter->indice_actual = indice;
    }

    return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter) {
    if (++iter->indice_actual >= iter->hash->dimension) return false;

    while (iter->hash->tabla_campos[iter->indice_actual].estado != OCUPADO) {
        iter->indice_actual++;
        if (hash_iter_al_final(iter)) return false;
    }

    return true;
}

const char* hash_iter_ver_actual(const hash_iter_t* iter) {
    if (hash_iter_al_final(iter)) return NULL;
    return iter->hash->tabla_campos[iter->indice_actual].clave;
}

bool hash_iter_al_final(const hash_iter_t* iter) {
    return iter->indice_actual >= iter->hash->dimension;
}

void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}