#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía, si ocurre un fallo devuelve NULL.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un elemento al principio de la lista
// Pre: la lista fue creada.
// Post: se devuelve veradero si se inserto correctamente, falso en caso contrario
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un elemento al final de la lista
// Pre: la lista fue creada.
// Post: se devuelve veradero si se inserto correctamente, falso en caso contrario
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista y devuelve un puntero al dato, si la lista esta vacia devuelve NULL
// Pre: la lista fue creada
// Post: el dato fue borrado de la lista
void *lista_borrar_primero(lista_t *lista);

// Devuelve un puntero al dato del primer elemento de la lista, si la lista esta vacia devuelve NULL
// Pre: la lista fue creada
void *lista_ver_primero(const lista_t *lista);

// Devuelve un puntero al dato del ultimo elemento de la lista, si la lista esta vacia devuelve NULL
// Pre: la lista fue creada
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista
// Pre: la lista fue creada
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro, para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void 
(*destruir_dato)(void *));

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera la lista con la funcion visitar como condicion de corte (si da false, corta la iteracion)
// Pre: la lista fue creada, la funcion visitar devuelve un bool al recibir un dato de la lista y no debe ser NULL
// El parametro extra se utilizará en la función
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador de la lista
// Pre: La lista debe ser creada
// Post: Devuelve un lista_iter_t iterador posicionado en la primer posicion de la lista, o NULL si esta vacia u ocurrio un error
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posicion en la lista, si no puede avanzar devuelve false
// Pre: la lista fue creada
// Post: devuelve true si se puede avanzar, false si esta en el final de la lista(si esta fuera)
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato que esta en la posicion del iterador
// Pre: el iterador fue creado
// Post: Devuelve el dato
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador esta al final de la lista (si esta fuera)
// Pre: el iterador fue creado
// Post: devuelve el valor booleano
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador lista_iter_t
// Pre: el iterador existe
// Post: destruye el iterador
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento entre el anterior y el actual
// Pre: el iterador fue creado
// Post: Instera el elemento entre el anterior y el actual y devuelve un bool para indicar si la operacion se concreto correctamente. El iterador queda en la posicion del insertado
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra un elemento y devuelve el dato
// Pre: el iterador fue creado
// Post: Devuelve el dato y borra el elemento que esta siendo apuntado por el iterador. El iterador queda en la siguiente posicion
void *lista_iter_borrar(lista_iter_t *iter);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);

#endif  // LISTA_H