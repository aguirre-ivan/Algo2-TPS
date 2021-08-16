#define _POSIX_C_SOURCE 200809L

#include "procesar_helper.h"
#include "hash.h"
#include "heap.h"
#include "count_min_sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* *****************************************************************
 *                        STRUCT TAG_T
 * *****************************************************************/

typedef struct tag {
	char* clave;		// tag
	size_t cantidad;	// cantidad de ocurrencias
} tag_t;

// Destruye y libera la memoria del tag_t
// Pre: el parametro recibido debe ser void*, y luego es casteado
// a tag_t
// Post: el tag_t fue destruido
void destruir_tag(void* tag) {
	tag_t* tag_cast = (tag_t*) tag;
	free(tag_cast->clave);
	free(tag);
}

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

// Funcion de comparacion
// Recibe 2 cadenas y devuelve 1 si la primera tiene mayor apariciones que
// la segunda (lo cual saca del contador sketch), -1 en caso contrario.
// En caso de tener misma cantidad de apariciones, devuelve en funcion
// del orden alfabetico
// Pre: Las cadenas recibidas deben ser void*
int funcion_cmp(const void* a, const void* b) {
	tag_t* tag_1 = (tag_t*) a;
	tag_t* tag_2 = (tag_t*) b;

	if (tag_1->cantidad == tag_2->cantidad) {
		return strcmp(tag_1->clave, tag_2->clave) > 0 ? -1 : 1;
	}

	return tag_1->cantidad > tag_2->cantidad ? 1 : -1;
}

// Imprime el top k de tags en el arreglo (midiendo en cuestion del sketch)
void imprimir_top_k(void** arreglo_tags, size_t len_arreglo, calc_num k) {
	heap_t* heap_tags = heap_crear_arr(arreglo_tags, len_arreglo, funcion_cmp);

	for (size_t i = 0; i < k; i++) {

		if (i == len_arreglo) break;

		tag_t* tag_desencolado = (tag_t*) heap_desencolar(heap_tags);
		size_t cantidad = tag_desencolado->cantidad;
		fprintf(stdout, "%ld %s\n", cantidad, tag_desencolado->clave);
		destruir_tag(tag_desencolado);

	}

	heap_destruir(heap_tags, destruir_tag);
}

// Procesa el tweet (la linea), aumenta el contador del tag y guarda
// el tag en el hash (set) de visitados.
void procesar_tweet(char** tweet, sketch_t* count_min_sketch, hash_t* hash_visitados) {

	size_t i = 1;
	while (tweet[i]) {
		sketch_aumentar_contador(count_min_sketch, tweet[i]);
		hash_guardar(hash_visitados, tweet[i], NULL);
		i++;
	}
}

// Recibe un hash en el que las claves son los tags iterados
// Itera las claves y devuelve un array de char* con las claves del hash
// Post: No destruye el hash, devuelve NULL en caso de fallar
void** hash_a_array(hash_t* hash_tags, size_t len_arreglo, sketch_t* sketch) {
	hash_iter_t* iter = hash_iter_crear(hash_tags);
	if (!iter) return NULL;

	void** resultado = malloc(sizeof(void*) * len_arreglo);
	if (!resultado) {
		hash_iter_destruir(iter);
		return NULL;
	}

	size_t i = 0;
	while (!hash_iter_al_final(iter)) {
		tag_t* tag = malloc(sizeof(tag_t));

		char* clave_dup = strdup(hash_iter_ver_actual(iter));
		if (!clave_dup) {
			hash_iter_destruir(iter);
			free(resultado);
			return NULL;
		}

		tag->clave = clave_dup;
		tag->cantidad = sketch_obtener_contador(sketch, clave_dup);
		resultado[i] = (void*) tag;

		i++;
		hash_iter_avanzar(iter);
	}

	hash_iter_destruir(iter);
	return resultado;
}

// Procesa la entrada en base a los parametros ingresados n y k
// Por cada n lineas imprime el top k de tags
// Recibe los dos parametros del tipo calc_num
void procesar_entrada(calc_num n, calc_num k) {

	char* linea = NULL;
	size_t tam = 0;

	sketch_t* count_min_sketch = crear_count_min_sketch();

	size_t contador_print = 1;
	bool ok = true;
	while (ok) {

		hash_t* tags_visitados = hash_crear(NULL);

		for (size_t i = 0; i < n; i++) {

			if (getline(&linea, &tam, stdin) == EOF) {
				ok = false;
				break;
			}

			char** tweet = split(linea, ',');
			procesar_tweet(tweet, count_min_sketch, tags_visitados);
			free_strv(tweet);

		}

		if (hash_cantidad(tags_visitados) != 0) {
			fprintf(stdout, "--- %ld\n", contador_print++);
		}

		size_t len_arreglo = hash_cantidad(tags_visitados);
		void** arreglo_visitados = hash_a_array(tags_visitados, len_arreglo, count_min_sketch);
		imprimir_top_k(arreglo_visitados, len_arreglo, k);
		hash_destruir(tags_visitados);
		free(arreglo_visitados);
	}

	destruir_count_min_sketch(count_min_sketch);
	free(linea);
} 

 /* *****************************************************************
 *                         FUNCION MAIN
 * *****************************************************************/

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "CANTIDAD DE PARAMETROS INVALIDOS\n");
		return 1;
	}

	char* n_char = argv[1];
	char* k_char = argv[2];
	calc_num n = 0;
	calc_num k = 0;

	if (!parse_num(n_char, &n) || !parse_num(k_char, &k)) {
		fprintf(stderr, "PARAMETRO INGRESADO INVALIDO\n");
		return 1;
	}

	procesar_entrada(n, k);

}