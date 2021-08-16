#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

 /* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

// Wrapper de strcmp
int _strcmp(const void* dato1, const void* dato2) {
    return strcmp((char*) dato1, (char*) dato2);
}

// Funcion de comparacion de enteros
int intcmp(const void* n1, const void* n2) {
    if (* (int*) n1 == * (int*) n2) return 0;
    return * (int*) n1 > * (int*) n2 ? 1 : -1;
}

 /* *****************************************************************
 *                      PRUEBAS HEAP
 * *****************************************************************/

static void prueba_crear_destruir_heap(void) {
    printf("\nINICIO PRUEBA SE PUEDE CREAR Y DESTRUIR HEAP (VACIO)\n");
    heap_t *heap = heap_crear(_strcmp);

    print_test("El heap fue creado", heap != NULL);

    heap_destruir(heap, NULL);
    printf("Heap destruido\n");
}

static void prueba_heap_vacio(void) {
    printf("\nINICIO DE PRUEBAS CON HEAP VACIO\n");

    heap_t *heap = heap_crear(_strcmp);
    
    print_test("El heap fue creado", heap != NULL);
    print_test("Heap esta vacio devuelve TRUE", heap_esta_vacio(heap));
    print_test("Ver primero del heap devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("Desencolar devuelve NULL", heap_desencolar(heap) == NULL);
    print_test("La cantidad es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
}


static void prueba_encolar_elementos(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN ENCOLAR ELEMENTOS\n");

    heap_t *heap = heap_crear(intcmp);
    int valores[] = {1, 2, 3};

    print_test("Ecolar elemento 0 es True", heap_encolar(heap, &valores[0]));
    print_test("El primero del heap es 0", heap_ver_max(heap) == &valores[0]);
    print_test("Ecolar elemento 1 es True", heap_encolar(heap, &valores[1]));
    print_test("El primero del heap es el elemento 1", heap_ver_max(heap) == &valores[1]);
    print_test("Ecolar elemento 2 es True", heap_encolar(heap, &valores[2]));
    print_test("El primero del heap es el elemento 2", heap_ver_max(heap) == &valores[2]);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("La cantidad es 3", heap_cantidad(heap) == 3);

    heap_destruir(heap, NULL);
}


static void prueba_encolar_null(void){
    printf("\nINICIO DE PRUEBAS SE PUEDE ENCOLAR NULL\n");
    heap_t* heap = heap_crear(_strcmp);

    print_test("Ecolar NULL es valido", heap_encolar(heap, NULL));
    print_test("El primero del heap es NULL", heap_ver_max(heap) == NULL);
    print_test("La cantidad es 1", heap_cantidad(heap) == 1);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}


static void prueba_desencolar_elementos(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN DESENCOLAR ELEMENTOS\n");

    printf("Encolar enteros\n");
    heap_t *heap_int = heap_crear(intcmp);
    int valores[] = {1, 10, 2, 9, 3, 8};
    bool ok = true;
    for (size_t i = 0; i < 6; i++) {
        ok = heap_encolar(heap_int, (void*) &valores[i]);
        if (!ok) break;
    }
    print_test("Se encolaron todos los elementos", ok);

    print_test("Desencolar elemento es el mayor", (int*) heap_desencolar(heap_int) == &valores[1]);
    print_test("Desencolar elemento es el mayor", (int*) heap_desencolar(heap_int) == &valores[3]);
    print_test("Desencolar elemento es el mayor", (int*) heap_desencolar(heap_int) == &valores[5]);
    print_test("Desencolar elemento es el mayor", (int*) heap_desencolar(heap_int) == &valores[4]);
    print_test("Desencolar elemento es el mayor", (int*) heap_desencolar(heap_int) == &valores[2]);
    print_test("Desencolar elemento es el mayor", (int*) heap_desencolar(heap_int) == &valores[0]);
    print_test("El primero del heap_int es NULL", (int*) heap_ver_max(heap_int) == NULL);
    print_test("El heap_int esta vacio", heap_esta_vacio(heap_int));

    heap_destruir(heap_int, NULL);

    printf("Encolar chars\n");
    heap_t *heap_char = heap_crear(_strcmp);
    char* clave1 = "perro";
    char* clave2 = "gato";
    char* clave3 = "vaca";

    ok = true;
    ok = heap_encolar(heap_char, (void*) clave1);
    ok = heap_encolar(heap_char, (void*) clave2);
    ok = heap_encolar(heap_char, (void*) clave3);
    print_test("Se encolaron todos los elementos", ok);
    print_test("La cantidad es 3", heap_cantidad(heap_char) == 3);
    
    print_test("El primero del heap_char es la clave3", (char*) heap_ver_max(heap_char) == clave3);
    print_test("Desencolar elemento es el mayor", (char*) heap_desencolar(heap_char) == clave3);
    print_test("Desencolar elemento es el mayor", (char*) heap_desencolar(heap_char) == clave1);
    print_test("Desencolar elemento es el mayor", (char*) heap_desencolar(heap_char) == clave2);
    print_test("La cantidad es 0", heap_cantidad(heap_char) == 0);

    heap_destruir(heap_char, NULL);
}


static void prueba_volumen(void){
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

    heap_t *heap = heap_crear(intcmp);
    int size_valores = 1000;
    int valores[size_valores];
    bool ok = true;

    printf("Encolando 1000 elementos..\n");
    for (int i = 0; i < size_valores; i++){
        valores[i] = i;
        if (!heap_encolar(heap, &valores[i])) {
            ok = false;
        }
    }

    print_test("El encolado de 1000 elementos fue correcto", ok);
    print_test("El primero del heap es el maximo", (int*) heap_ver_max(heap) == &valores[999]);
    
    ok = true;
    printf("Desencolando 1000 elementos..\n");
    for (int j = 999; j >= 0; j--) {
        if (heap_desencolar(heap) != &valores[j]) ok = false;
        if (!ok) break;
    }
    print_test("Los elementos desencolados fueron correctos", ok);
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("El max del heap es NULL", heap_ver_max(heap) == NULL);
    
    heap_destruir(heap, NULL);
}


static void prueba_funcion_destruir_free(void){
    printf("\nINICIO DE PRUEBAS DESTRUIR COLA (CON FUNCION FREE)\n");

    heap_t* heap = heap_crear(intcmp);

    int* entero1 = malloc(sizeof(int*));
    int* entero2 = malloc(sizeof(int*));
    int* entero3 = malloc(sizeof(int*));
    int* entero4 = malloc(sizeof(int*));


    if (!(entero1 && entero2 && entero3 && entero4)) {
        printf("ERROR EN CREACION DE PRUEBA\n");
        free(entero1);
        free(entero2);
        free(entero3);
        free(entero4);
    } else {
        *entero1 = 1;
        *entero2 = 2;
        *entero3 = 3;
        *entero4 = 4;
        printf("Encolando elementos en memoria dinamica a destruir\n");
        print_test("Se encolo el primer elemento", heap_encolar(heap, entero1));
        print_test("Se encolo el segundo elemento", heap_encolar(heap, entero2));
        print_test("Se encolo el tercer elemento", heap_encolar(heap, entero3));
        print_test("Se encolo el cuarto elemento", heap_encolar(heap, entero4));
    }
    heap_destruir(heap, free);
    printf("El heap fue destruido\n");
}

static void prueba_crear_heap_con_arreglo(void){
    printf("\nINICIO DE PRUEBAS CREAR HEAP CON ARREGLO\n");

    int valores[] = {1, 6, 2, 5, 3, 4};
    void* arreglo[6];
    int valores2[] = {0, 10};

    for (size_t i = 0; i < 6; i++) arreglo[i] = (void*) &valores[i];

    heap_t* heap = heap_crear_arr(arreglo, 6, intcmp);

    print_test("El heap fue creado", heap);
    print_test("La cantidad es 6", heap_cantidad(heap) == 6);

    print_test("Se pueden encolar mas elementos", heap_encolar(heap, (void*)&valores2[0]));
    print_test("Se pueden encolar mas elementos", heap_encolar(heap, (void*)&valores2[1]));

    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores2[1]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores[1]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores[3]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores[5]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores[4]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores[2]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores[0]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores2[0]);
    print_test("El heap esta vacio", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

static void prueba_crear_heap_con_arreglo_volumen(void) {
    printf("\nINICIO DE PRUEBA CREAR HEAP CON ARREGLO EN VOLUMEN\n");

    int largo = 1000;
    int valores_vol[largo];
    void* arreglo_vol[largo];

    int valores2[] = {1256, 1786};

    for (size_t i = 0; i < largo; i++) {
        valores_vol[i] = (int)i;
    }

    for (size_t i = 0; i < largo; i++) {
        arreglo_vol[i] = (void*) &valores_vol[i];
    }

    for (size_t i = 0; i < largo; i++) {
        size_t n_rand = rand() % (largo - 1);
        int aux = *(int*)arreglo_vol[i];
        *(int*)arreglo_vol[i] = *(int*)arreglo_vol[n_rand];
        *(int*)arreglo_vol[n_rand] = aux;
    }

    printf("Se creo arreglo en volumen desordenado..\n");

    heap_t* heap = heap_crear_arr(arreglo_vol, largo, intcmp);

    print_test("El heap fue creado", heap);
    print_test("La cantidad es correcta", heap_cantidad(heap) == largo);

    print_test("Se pueden encolar mas elementos", heap_encolar(heap, (void*)&valores2[0]));
    print_test("Se pueden encolar mas elementos", heap_encolar(heap, (void*)&valores2[1]));

    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores2[1]);
    print_test("Desencolar elemento es el mayor", heap_desencolar(heap) == &valores2[0]);

    bool ok = true;
    for (int i = largo-1; i >= 0; i--) { //largo-1 por que empieza en 0 y va hasta 999 entonces el max va a ser 999.
        ok &= (*(int*)heap_desencolar(heap) == i);
        if (!ok) break;
    }

    print_test("Se desencolaron todos los elementos y esos eran siempre los mayores del heap", ok);
    print_test("El heap esta vacio", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
    printf("El heap fue destruido\n");
}

 /* *****************************************************************
 *                      PRUEBAS HEAPSORT
 * *****************************************************************/

static void prueba_heapsort(void){
    printf("\nINICIO DE PRUEBAS HEAPSORT\n");

    printf("Se creo arreglo desordenado..\n");
    int valores[] = {1, 6, 2, 5, 3, 4};
    void* arreglo[6];
    for (size_t i = 0; i < 6; i++) arreglo[i] = (void*) &valores[i];

    heap_sort(arreglo, 6, intcmp);

    bool ok = true;
    for (int i = 0; i < 6; i++) {
        if (*(int*)arreglo[i] != i+1) ok = false;
        if (!ok) break;
    }

    print_test("Al aplicar heapsort el arreglo esta ordenado", ok);

}

static void prueba_heapsort_volumen(void){
    printf("\nINICIO DE PRUEBAS HEAPSORT VOLUMEN\n");

    size_t largo = 1000;
    int valores_vol[largo];
    void* arreglo_vol[largo];

    for (size_t i = 0; i < largo; i++) {
        valores_vol[i] = (int)i;
    }

    for (size_t i = 0; i < largo; i++) {
        arreglo_vol[i] = (void*) &valores_vol[i];
    }

    for (size_t i = 0; i < largo; i++) {
        size_t n_rand = rand() % (largo - 1);
        int aux = *(int*)arreglo_vol[i];
        *(int*)arreglo_vol[i] = *(int*)arreglo_vol[n_rand];
        *(int*)arreglo_vol[n_rand] = aux;
    }

    printf("Se creo arreglo en volumen desordenado..\n");

    heap_sort(arreglo_vol, largo, intcmp);

    bool ok = true;
    for (int i = 0; i < largo; i++) {
        if (*(int*)arreglo_vol[i] != i) ok = false;
        if (!ok) break;
    }

    print_test("Se aplico heapsort al arreglo desordenado y lo devolvio ordenado", ok);

}

void pruebas_heap_estudiante() {
    prueba_crear_destruir_heap();
    prueba_heap_vacio();
    prueba_encolar_elementos();
    prueba_encolar_null();
    prueba_desencolar_elementos();
    prueba_volumen();
    prueba_funcion_destruir_free();
    prueba_crear_heap_con_arreglo();
    prueba_crear_heap_con_arreglo_volumen();
    prueba_heapsort();
    prueba_heapsort_volumen();
}

#ifndef CORRECTOR

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;
}

#endif
