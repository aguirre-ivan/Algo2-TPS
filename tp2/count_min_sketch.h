#ifndef _COUNT_MIN_SKETCH_H
#define _COUNT_MIN_SKETCH_H

#include <stdlib.h>
#include <stdbool.h>

/* *****************************************************************
 *                 DEFINICION DE ESTRUCTURA
 * *****************************************************************/

typedef struct count_min_sketch sketch_t;

/* *****************************************************************
 *                       PRIMITIVAS
 * *****************************************************************/

// Crea un count min sketch
// Post: Devuelve el puntero al sketch_t o NULL en caso de fallar
sketch_t* crear_count_min_sketch();

// Aumenta el contador de la clave recibida
void sketch_aumentar_contador(sketch_t* sketch, char* clave);

// Devuelve el contador de la clave recibida
size_t sketch_obtener_contador(sketch_t* sketch, char* clave);

// Destruye el count min sketch
void destruir_count_min_sketch(sketch_t* sketch);

#endif  // _COUNT_MIN_SKETCH_H