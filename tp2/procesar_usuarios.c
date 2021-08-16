#define _POSIX_C_SOURCE 200809L
#include "procesar_helper.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_USUARIO 15
#define CHAR_MIN 0
#define CHAR_MAX 126
#define COUNTING_OCURRENCIAS -1

/* *****************************************************************
 *                      STRUCT USUARIO
 * *****************************************************************/

typedef struct usuario {
    char* nombre;       // cadena de tamaño TAM_USUARIO
    size_t n_tags;      // numero de tags utilizados por el usuario
} usuario_t;

// Crea un struct usuario_t, asignando el nombre y numero de tags recibido
// por parametro. NO se encarga de liberar memoria del nombre guardado
// Crea una copia de la cadena recibida en una cadena inicializada
// en \0 de tamaño TAM_USUARIO
usuario_t* crear_usuario_t(const char* nombre, size_t numero_tags) {
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (!usuario) return NULL;

    char* cadena_nombre = calloc(TAM_USUARIO, sizeof(char)+1);
    if (!cadena_nombre) {
        free(usuario);
        return NULL;
    }

    stpcpy(cadena_nombre, nombre);

    usuario->nombre = cadena_nombre;
    usuario->n_tags = numero_tags;

    return usuario;
}

// Actualiza el minimo de tags en el puntero recibido
void actualizar_min_tags(size_t* min_tags, size_t cantidad_tags) {
    if (cantidad_tags < *min_tags) *min_tags = cantidad_tags;
}

// Actualiza el maximo de tags en el puntero recibido
void actualizar_max_tags(size_t* max_tags, size_t cantidad_tags) {
    if (*max_tags < cantidad_tags) *max_tags = cantidad_tags;
}

// Convierte el hash recibido a una lista de usuarios_t
// PRE: el hash recibido tiene como claves los usuarios y cada 
// uno tiene como dato un hash con los tags utilizados
// POST: Se encarga de la destruccion del hash
usuario_t** hash_a_lista_usuarios_t(hash_t* hash_usuarios, size_t* min_tags, size_t* max_tags) {
    size_t cantidad = hash_cantidad(hash_usuarios);

    usuario_t** lista_usuarios = malloc(sizeof(usuario_t*) * cantidad);
    if (!lista_usuarios) return NULL;
    
    hash_iter_t* iter = hash_iter_crear(hash_usuarios);
    if (!iter) {
        free(lista_usuarios);
        return NULL;
    }

    size_t i = 0;
    while (!hash_iter_al_final(iter)) {

        const char* nombre = hash_iter_ver_actual(iter);     
        hash_t* hash_tags = (hash_t*) hash_obtener(hash_usuarios, nombre);
        size_t cantidad_tags = hash_cantidad(hash_tags);
        usuario_t* usuario = crear_usuario_t(nombre, cantidad_tags);
        lista_usuarios[i] = usuario;

        actualizar_min_tags(min_tags, cantidad_tags);
        actualizar_max_tags(max_tags, cantidad_tags);

        hash_iter_avanzar(iter);
        i++;
    }

    hash_iter_destruir(iter);
    hash_destruir(hash_usuarios);
    return lista_usuarios;
}

// Destruye la lista de usuarios_t recibida por parametro
void destruir_lista_usuarios_t(usuario_t** lista_usuarios, size_t len) {
    for (size_t i = 0; i < len; i++) {
        free(lista_usuarios[i]->nombre);
        free(lista_usuarios[i]);
    }
    free(lista_usuarios);
}

/* *****************************************************************
 *                      FUNCIONES DE ORDENAMIENTO
 * *****************************************************************/

// Recibe una lista de usuarios_t, el indice_k que indica el caracter que
// se tomara en cuenta para comparar, y guarda los caracteres minimo
// y maximo en los punteros size_t* recibidos
void calcular_rango_chars(usuario_t** lista_a_ordenar, size_t len_usuarios, size_t indice_k, size_t* minimo_char, size_t* maximo_char) {
    for (size_t i = 0; i < len_usuarios; i++) {
        char* usuario = lista_a_ordenar[i]->nombre;
        if ((size_t) usuario[indice_k] < *minimo_char) *minimo_char = (size_t) usuario[indice_k];
        if ((size_t) usuario[indice_k] > *maximo_char) *maximo_char = (size_t) usuario[indice_k];
    }
}

void counting_sort(usuario_t** lista_ordenar, size_t len_usuarios, int indice_k, size_t min, size_t max) {

    size_t k = max - min;
    if (k == 0) return;
   
    size_t contador[k+1];
    for (size_t i = 0; i < (k+1); i++) contador[i] = 0;

    for (size_t i = 0; i < len_usuarios; i++) {
        size_t i_ocurrencia = indice_k == COUNTING_OCURRENCIAS ? lista_ordenar[i]->n_tags : (size_t) (lista_ordenar[i]->nombre[indice_k]);
        contador[i_ocurrencia-min]++;
    }

    size_t suma_acumulada[k+1];
    suma_acumulada[0] = 0;
    for (size_t i = 1; i < (k+1); i++) {
        size_t cantidad = contador[i-1];
        suma_acumulada[i] = suma_acumulada[i-1] + cantidad;
    }

    usuario_t** lista_resultado = malloc(sizeof(usuario_t*) * len_usuarios);
    if (!lista_resultado) return;

    for (size_t i = 0; i < len_usuarios; i++) {

        usuario_t* valor = lista_ordenar[i];
        size_t j = indice_k == COUNTING_OCURRENCIAS ? valor->n_tags - min : (size_t) valor->nombre[indice_k] - min;

        size_t pos = suma_acumulada[j];
        suma_acumulada[j]++;

        lista_resultado[pos] = valor;

    }

    for (size_t i = 0; i < len_usuarios; i++) {
        lista_ordenar[i] = lista_resultado[i];
    }

    free(lista_resultado);
}

// Ordena el arreglo de usuarios_t recibido respecto a sus ocurrencias de
// cantidad de tags utilizados. Recibe ya el minimo y maximo de cantidad
// de tags.
// Post: actualiza el arreglo recibido.
void counting_ocurrencias(usuario_t** lista_usuarios, size_t len_usuarios, size_t min, size_t max) {
    counting_sort(lista_usuarios, len_usuarios, COUNTING_OCURRENCIAS, min, max);
}

// Ordena el arreglo de usuarios_t recibido en orden alfabetico.
// Post: actualiza el arreglo recibido.
void radix_alfabetico(usuario_t** lista_usuarios, size_t len_usuarios) {
    for (size_t i = 0; i <= TAM_USUARIO; i++) {
        int indice_k = TAM_USUARIO - (int) i;
        size_t minimo_char = CHAR_MAX;
        size_t maximo_char = CHAR_MIN;

        calcular_rango_chars(lista_usuarios, len_usuarios, indice_k, &minimo_char, &maximo_char);

        counting_sort(lista_usuarios, len_usuarios, indice_k, minimo_char, maximo_char);
    }
}

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

// Procesa el tweet (la linea recibida como un arreglo strv), guarda
// el usuario como clave de diccionario en el hash_usuarios y como dato
// otro hash con los tags utilizados guardados como claves.
void procesar_tweet(char** tweet, hash_t* hash_usuarios) {
    size_t i = 0;
    char* usuario = tweet[i];

    if (!hash_pertenece(hash_usuarios, usuario)) {
        hash_t* tags_usuario = hash_crear(NULL);
        hash_guardar(hash_usuarios, usuario, (void*) tags_usuario);
    }

    i++;
    
	while (tweet[i]) {
        char* tag = tweet[i];
        hash_t* tags_usuario = hash_obtener(hash_usuarios, usuario);
        hash_guardar(tags_usuario, tag, NULL);
		i++;
	}

}

// Recibe una lista de usuarios_t ordenada por cantidad de tags utilizados
// e imprime por pantalla la cantidad de tags y todos los usuarios que
// utilizaron dicha cantidad.
void imprimir_usuarios(usuario_t** lista_usuarios, size_t len_usuarios) {
    if (len_usuarios == 0) return;
    size_t cantidad_actual = lista_usuarios[0]->n_tags;
    fprintf(stdout, "%ld: ", cantidad_actual);

    for (int i = 0; i < len_usuarios; i++) {
        
        size_t cantidad_tags = lista_usuarios[i]->n_tags;

        if (cantidad_actual != cantidad_tags) {
            cantidad_actual = cantidad_tags;
            fprintf(stdout, "\n%ld: ", cantidad_actual);
        } else if (i != 0) {
            fprintf(stdout, ", ");
        }

        fprintf(stdout, "%s", lista_usuarios[i]->nombre);
    }

    fprintf(stdout, "\n");
}

// Recibido un hash imprime los usuarios en orden de tags utilizados
// Y en caso de tener la misma cantidad de tags respeta orden
// alfabetico
void print_usuarios_ordenados(hash_t* usuarios) {
    size_t cant_usuarios = hash_cantidad(usuarios);
    size_t min_tags = 1;
    size_t max_tags = 1;
    
    usuario_t** lista_usuarios_t = hash_a_lista_usuarios_t(usuarios, &min_tags, &max_tags);

    radix_alfabetico(lista_usuarios_t, cant_usuarios);
    counting_ocurrencias(lista_usuarios_t, cant_usuarios, min_tags, max_tags);
    imprimir_usuarios(lista_usuarios_t, cant_usuarios);
    destruir_lista_usuarios_t(lista_usuarios_t, cant_usuarios);
}

// Recibido el archivo por parametro y un hash vacio procesa el archivo
// POST: el hash quedara con claves los usuarios y cada 
// uno tiene como dato un hash con los tags utilizados
// Se encarga de cerrar el archivo
void procesar_archivo(FILE* f_entrada, hash_t* usuarios) {
    char* linea = NULL;
	size_t tam = 0;

    while (getline(&linea, &tam, f_entrada) != EOF) {
		char** tweet = split(linea, ',');
		procesar_tweet(tweet, usuarios);
        free_strv(tweet);
	}

    free(linea);
    fclose(f_entrada);
}

/* *****************************************************************
 *                      FUNCION MAIN
 * *****************************************************************/

int main(int argc, char** argv) {
    if (argc != 2) {
		fprintf(stderr, "CANTIDAD DE PARAMETROS INVALIDOS\n");
        return 1;
	}

	char* nombre_archivo = argv[1];

    FILE* f_entrada = fopen(nombre_archivo, "r");
    if (!f_entrada) {
		fprintf(stderr, "ERROR EN ABRIR ARCHIVO\n");
        return 1;
    }

    hash_t* usuarios = hash_crear(_hash_destruir);
    procesar_archivo(f_entrada, usuarios);

    print_usuarios_ordenados(usuarios);

    return 0;
}