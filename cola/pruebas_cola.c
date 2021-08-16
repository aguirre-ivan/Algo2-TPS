#include "cola.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static void prueba_crear_destruir_cola(void) {
    printf("\nINICIO PRUEBA SE PUEDE CREAR Y DESTRUIR COLA (VACIA)\n");
    cola_t *cola = cola_crear();

    print_test("La cola fue creada", cola != NULL);

    cola_destruir(cola, NULL);
    printf("Cola destruida\n");
}

static void prueba_cola_vacia(void) {
    printf("\nINICIO DE PRUEBAS CON COLA VACIA\n");

    cola_t *cola = cola_crear();
    
    print_test("La cola fue creada", cola != NULL);
    print_test("Cola esta vacia devuelve TRUE", cola_esta_vacia(cola));
    print_test("Ver primero de la cola devuelve NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolar devuelve NULL", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
}


static void prueba_encolar_elementos(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN ENCOLAR ELEMENTOS\n");

    cola_t *cola = cola_crear();
    printf("Pruebas encolar enteros\n");
    int valores[3];

    print_test("Ecolar elemento 0 es True", cola_encolar(cola, &valores[0]));
    print_test("El primero de la cola es 0", cola_ver_primero(cola) == &valores[0]);
    print_test("Ecolar elemento 1 es True", cola_encolar(cola, &valores[1]));
    print_test("Ecolar elemento 2 es True", cola_encolar(cola, &valores[2]));
    print_test("El primero de la cola es 0", cola_ver_primero(cola) == &valores[0]);
    print_test("La cola no esta vacia", !cola_esta_vacia(cola));

    cola_destruir(cola, NULL);
}


static void prueba_encolar_null(void){
    printf("\nINICIO DE PRUEBAS SE PUEDE ENCOLAR NULL\n");
    cola_t* cola = cola_crear();

    print_test("Ecolar NULL es valido", cola_encolar(cola, NULL));
    print_test("El primero de la cola es NULL", cola_ver_primero(cola) == NULL);
    print_test("La cola no esta vacia", !cola_esta_vacia(cola));

    cola_destruir(cola, NULL);
}


static void prueba_desencolar_elementos(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN DESENCOLAR ELEMENTOS\n");

    cola_t *cola = cola_crear();
    int valores[3];

    for (int i = 0; i < 3; i++) {
        cola_encolar(cola, &valores[i]);
    }
    
    print_test("Se desencolo el elemento 0", cola_desencolar(cola) == &valores[0]);
    print_test("Se desencolo el elemento 1", cola_desencolar(cola) == &valores[1]);
    print_test("Se desencolo el elemento 2", cola_desencolar(cola) == &valores[2]);

    print_test("La cola esta vacia", cola_esta_vacia(cola));
    print_test("Ver el primero de la cola devuelve NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolar cola vacia devuelve NULL", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
}


static void prueba_volumen(void){
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

    cola_t *cola = cola_crear();
    int size_valores = 1000;
    int valores[size_valores];
    bool ok = true;

    printf("Encolando 1000 elementos..\n");
    for (int i = 0; i < size_valores; i++){
        if (!cola_encolar(cola, &valores[i])) {
            ok = false;
        }
    }

    print_test("El encolado de 1000 elementos fue correcto", ok);
    print_test("El primero de la cola es 0", cola_ver_primero(cola) == &valores[0]);
    
    ok = true;
    printf("Desencolando 1000 elementos..\n");
    for (int j = 0; j < size_valores; j++) {
        if (cola_desencolar(cola) != &valores[j]) {
            ok = false;
        }
    }

    print_test("Los elementos desencolados fueron correctos", ok);
    print_test("La cola esta vacia", cola_esta_vacia(cola));
    print_test("El primero de la cola es NULL", cola_ver_primero(cola) == NULL);
    
    cola_destruir(cola, NULL);
}


static void prueba_funcion_destruir_free(void){
    printf("\nINICIO DE PRUEBAS DESTRUIR COLA (CON FUNCION FREE)\n");

    cola_t *cola = cola_crear();

    int* entero1 = malloc(sizeof(int*));
    int* entero2 = malloc(sizeof(int*));

    char* string1 = malloc(sizeof(char*));
    char* string2 = malloc(sizeof(char*));

    if (!(entero1 && entero2 && string1 && string2)){
        printf("ERROR EN CREACION DE PRUEBA\n");
        free(entero1);
        free(entero2);
        free(string1);
        free(string2);
    } else { 
        printf("Encolando elementos en memoria dinamica a destruir\n");
        print_test("Se encolo el primer elemento", cola_encolar(cola, entero1));
        print_test("Se encolo el segundo elemento", cola_encolar(cola, entero2));
        print_test("Se encolo el tercer elemento", cola_encolar(cola, string1));
        print_test("Se encolo el cuarto elemento", cola_encolar(cola, string2));
        
    }
    cola_destruir(cola, free);
    printf("La cola fue destruida\n");
}

    
static void prueba_funcion_destruir_otra_funcion(void){
    printf("\nINICIO DE PRUEBAS DESTRUIR COLA (CON OTRA FUNCION)\n");

    pila_t* pila1 = pila_crear();
    pila_t* pila2 = pila_crear();
    pila_t* pila3 = pila_crear();

    if(!pila1 || !pila2 || !pila3){
        pila_destruir(pila1);
        pila_destruir(pila2);
        pila_destruir(pila3);
        printf("Error en creacion de pilas de prueba\n");
    } else {
    printf("Pilas de prueba creadas\n");
    }

    cola_t* cola = cola_crear();

    print_test("Encolar pila1", cola_encolar(cola, pila1));
    print_test("Encolar pila2", cola_encolar(cola, pila2));
    print_test("Encolar pila3", cola_encolar(cola, pila3));

    cola_destruir(cola, wrapper_pila_destruir);
    printf("Cola destruida\n");
}


void pruebas_cola_estudiante() {
    prueba_crear_destruir_cola();
    prueba_cola_vacia();
    prueba_encolar_elementos();
    prueba_encolar_null();
    prueba_desencolar_elementos();
    prueba_volumen();
    prueba_funcion_destruir_free();
    prueba_funcion_destruir_otra_funcion();
}

#ifndef CORRECTOR

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;
}

#endif
