#ifndef _PROCESAR_HELPER
#define _PROCESAR_HELPER

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef int64_t calc_num;

// Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
void free_strv(char* strv[]);

// Convierte una cadena a entero, indicando si se pudo.
//
// Args:
//    tok: la cadena a convertir
//    dest: dónde guardar el valor convertido
//
// Returns:
//    verdadero si la cadena era en su totalidad un entero válido.
bool parse_num(const char *tok, calc_num *dest);

/*
 * Devuelve en un arreglo dinámico terminado en NULL todos los subsegmentos de
 * ‘str’ separados por el carácter ‘sep’. Tanto el arreglo devuelto como las
 * cadenas que contiene se ubicarán en nuevos espacios de memoria dinámica.
 *
 * La liberación de la memoria dinámica devuelta queda a cargo de quien llame a
 * esta función. La función devuelve NULL en caso de error.
 */
char **split(const char *str, char sep);

#endif  // _PROCESAR_HELPER