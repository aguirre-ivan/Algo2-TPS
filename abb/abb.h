#ifndef _ABB_H
#define _ABB_H
#include <stdbool.h>
#include <stdlib.h>

/* *****************************************************************
 *                DEFINICION DE ESTRUCTURAS
 * *****************************************************************/

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);

typedef struct abb_iter abb_iter_t;

/* *****************************************************************
 *                PRIMITIVAS DEL ABB
 * *****************************************************************/

// Crea un nuevo ABB, en caso de no poder, devuelve NULL
// Pre: recibe una funcion para comparacion de claves, y opcionalmente
// una funcion de destruccion.
// Post: devuelve el abb creado
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Asocia una clave con un dato, la clave es agregada de manera ordenada
// Si la clave existe, se reemplaza el dato (utilizando funcion destruir_dato)
// Pre: el abb existe
// Post: devuelve un booleano indicando si se pudo lograr o no
bool abb_guardar(abb_t* arbol, const char* clave, void* dato);

// Busca una clave, la borra y devuelve el dato asociado a dicha clave.
// En caso de no existir la clave, devuelve NULL
// Pre: el abb existe
// Post: Devuelve el dato asociado a la clave borrada, o NULL si no existe la clave.
void* abb_borrar(abb_t* arbol, const char* clave);

// Devuelve el dato asociado a la clave, en caso de no existir devuelve NULL
// Pre: el abb existe
void* abb_obtener(const abb_t* arbol, const char* clave);

// Devuelve un booleano que indica si la clave pertenece al abb
// Pre: el abb existe
bool abb_pertenece(const abb_t* arbol, const char* clave);

// Devuelve la cantidad de claves del abb
// Pre: el abb existe
size_t abb_cantidad(const abb_t* arbol);

// Destruye el abb utilizando la funcion destruir_dato
// Pre: el abb existe
// Post: el abb fue destruido
void abb_destruir(abb_t* arbol);

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera internamente el abb de manera in order
// Para cada elemento del arbol aplica la funcion visitar, y si esta
// devuelve false deja de iterar
// Pre: el arbol existe
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra);

/* *****************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador externo (inorder)
// Pre: el abb existe
// Post: devuelve el iterador, o NULL si no se pudo crear
abb_iter_t* abb_iter_in_crear(const abb_t* arbol);

// Avanza una posicion, en caso de estar al final no avanza
// Pre: el iterador existe
// Post: devuelve un booleano indicando si se avanzo o no
bool abb_iter_in_avanzar(abb_iter_t* iter);

// Devuelve la clave a la que apunta el iterador, en caso de estar
// al final, devuelve NULL
// Pre: el iterador existe
const char* abb_iter_in_ver_actual(const abb_iter_t* iter);

// Pre: el iterador existe
// Post: devuelve un booleano que indica si el iterador esta al final
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Pre: el iterador existe
// Post: el iterador fue destruido
void abb_iter_in_destruir(abb_iter_t* iter);

#endif  // _ABB_H