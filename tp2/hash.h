#ifndef _HASH_H
#define _HASH_H

#include <stdlib.h>
#include <stdbool.h>

/* *****************************************************************
 *                DEFINICION DE ESTRUCTURAS
 * *****************************************************************/

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destruir_dato_t)(void*);

/* *****************************************************************
 *                PRIMITIVAS DEL HASH
 * *****************************************************************/

// Crea un hash
// Pre: Recibe funcion para destruir los datos del hash
// Post: Devuelve el hash creado o NULL si falla
hash_t* hash_crear(hash_destruir_dato_t destruir_dato);

// Guarda un dato en el hash asociado a la clave.
// Si la clave esta en el hash la reemplaza (destruye la anterior
// y guarda la nueva).
// Devuelve true si se guardo correctamente, False en caso contrario
// Pre: el hash fue creado
bool hash_guardar(hash_t *hash, const char* clave, void* dato);

// Borra la clave del hash y devuelve el dato asociado, si no existe
// se devuelve NULL.
// Pre: el hash existe
// Post: se borro la clave y se devolvio el dato 
void* hash_borrar(hash_t* hash, const char* clave);

// Devuelve el dato asociado a la clave, devuelve NULL en caso
// de no existir la clave
// Pre: el hash existe
void* hash_obtener(const hash_t* hash, const char* clave);

// Se fija si la clave existe en el hash y devuelve un booleano
// Pre: el hash existe
bool hash_pertenece(const hash_t* hash, const char* clave);

// Devuelve la cantidad de claves en el hash.
// Pre: el hash existe
size_t hash_cantidad(const hash_t* hash);

// Destruye el hash, aplicando a los datos la funcion de destruccion
// que recibio al ser creado, si la funcion era NULL no les hace nada
// Pre: el hash existe
// Post: el hash fue destruido
void hash_destruir(hash_t* hash);

// Wrapper de funcion hash_destruir
void _hash_destruir(void* hash);

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// Crea un iterador de hash
// Pre: el hash existe
// Post: devuelve un hash_iter_t iterador posicionado en la primer
// posicion (al final si esta vacio el hash), o NULL si ocurrio un 
// error en la creacion del iterador
hash_iter_t* hash_iter_crear(const hash_t* hash);

// Avanza una posicion del iterador
// Pre: el hash fue creada
// Post: devuelve true si se puede avanzar, false si esta en el
// final (si esta fuera)
bool hash_iter_avanzar(hash_iter_t* iter);

// Devuelve el elemento al que apunta el iterador o NULL si el
// esta al final (esta fuera de la lista)
// Pre: el iterador fue creado
const char* hash_iter_ver_actual(const hash_iter_t* iter);

// Devuelve true si el iterador recorrio todos los elementos 
// y esta fuera del hash
// Pre: el iterador fue creado
bool hash_iter_al_final(const hash_iter_t* iter);

// Pre: el iterador fue creado
// Post: el iterador fue destruido
void hash_iter_destruir(hash_iter_t* iter);

#endif  // _HASH_H