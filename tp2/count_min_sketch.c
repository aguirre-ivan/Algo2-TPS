#include "count_min_sketch.h"
#define TAM 503

/* *****************************************************************
 *                 DEFINICION DE ESTRUCTURA
 * *****************************************************************/

struct count_min_sketch {
    size_t* tabla1;
    size_t* tabla2;
    size_t* tabla3;
};

/* *****************************************************************
 *                     FUNCIONES DE HASHING
 * *****************************************************************/

// Funciones de hashing de http://profesores.elo.utfsm.cl/~agv/elo320/miscellaneous/hashFunction/hashFunction.html

static unsigned long sdbm(unsigned char *str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

unsigned long djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

// Funcion de hashing de https://azrael.digipen.edu/~mmead/www/Courses/CS280/HashFunctions-1.html

unsigned djb1(unsigned char *key) {
    unsigned hash = 0;
    while (*key) {
        hash *= 33;
        hash += *key;
        key++;
    }
  return hash;
}

// Wrappers de las funciones de hashing para castear size_t

size_t f_hashing_1(unsigned char *str) {
    return (size_t) (sdbm(str));
}

size_t f_hashing_2(unsigned char *str) {
    return (size_t) (djb2(str));
}

size_t f_hashing_3(unsigned char *str) {
    return (size_t) (djb1(str));
}

/* *****************************************************************
 *                     FUNCIONES AUXILIARES
 * *****************************************************************/

size_t obtener_minimo(size_t valor1, size_t valor2, size_t valor3) {
    size_t primer_comp = valor1 < valor2 ? valor1 : valor2;
    return primer_comp < valor3 ? primer_comp : valor3;
}

/* *****************************************************************
 *                         PRIMITIVAS
 * *****************************************************************/

sketch_t* crear_count_min_sketch() {
    sketch_t* sketch = malloc(sizeof(sketch_t));
    if (!sketch) return NULL;

    size_t* t1 = malloc(sizeof(size_t) * TAM);
    if (!t1) {
        free(sketch);
        return NULL;
    }

    size_t* t2 = malloc(sizeof(size_t) * TAM);
    if (!t2) {
        free(sketch);
        free(t1);
        return NULL;
    }

    size_t* t3 = malloc(sizeof(size_t) * TAM);
    if (!t3) {
        free(sketch);
        free(t1); free(t2);
        return NULL;
    }

    for (size_t i = 0; i < TAM; i++) {
        t1[i] = 0;
        t2[i] = 0;
        t3[i] = 0;
    }

    sketch->tabla1 = t1;
    sketch->tabla2 = t2;
    sketch->tabla3 = t3;

    return sketch;
}

void sketch_aumentar_contador(sketch_t* sketch, char* clave) {
    size_t i1 = f_hashing_1((unsigned char*) clave) % TAM;
    size_t i2 = f_hashing_2((unsigned char*) clave) % TAM;
    size_t i3 = f_hashing_3((unsigned char*) clave) % TAM;

    sketch->tabla1[i1]++;
    sketch->tabla2[i2]++;
    sketch->tabla3[i3]++;
}

size_t sketch_obtener_contador(sketch_t* sketch, char* clave) {
    size_t i1 = f_hashing_1((unsigned char*) clave) % TAM;
    size_t i2 = f_hashing_2((unsigned char*) clave) % TAM;
    size_t i3 = f_hashing_3((unsigned char*) clave) % TAM;

    size_t cont1 = sketch->tabla1[i1];
    size_t cont2 = sketch->tabla2[i2];
    size_t cont3 = sketch->tabla3[i3];

    return obtener_minimo(cont1, cont2, cont3);
}

void destruir_count_min_sketch(sketch_t* sketch) {
    free(sketch->tabla1);
    free(sketch->tabla2);
    free(sketch->tabla3);
    free(sketch);
}