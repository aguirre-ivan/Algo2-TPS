#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdbool.h>


static void prueba_crear_destruir_pila(void){
    printf("\nINICIO PRUEBA SE PUEDE CREAR Y DESTRUIR PILA\n");
    pila_t *pila = pila_crear();

    print_test("La pila fue creada", pila != NULL);

    pila_destruir(pila);
    printf("Pila destruida\n");
}


static void prueba_pila_vacia(void) {
    printf("\nINICIO DE PRUEBAS CON PILA VACIA\n");

    pila_t *pila = pila_crear();
    
    print_test("La pila fue creada", pila != NULL);
    print_test("Pila esta vacia devuelve TRUE", pila_esta_vacia(pila));
    print_test("Ver tope en pila vacia devuelve NULL", pila_ver_tope(pila) == NULL);
    print_test("Desapilar en pila vacia devuelve NULL", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
}


static void prueba_apilar_elementos(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN APILAR ELEMENTOS\n");

    pila_t *pila = pila_crear();
    int valores[3];

    print_test("Apilar elemento 0 es true", pila_apilar(pila, &valores[0]));print_test("El tope es 0 es true", pila_ver_tope(pila) == &valores[0]);
    print_test("Apilar elemento 1 es true", pila_apilar(pila, &valores[1]));
    print_test("El tope es 1 es true", pila_ver_tope(pila) == &valores[1]);
    print_test("Apilar elemento 2 es true", pila_apilar(pila, &valores[2]));
    print_test("El tope es 2 es true", pila_ver_tope(pila) == &valores[2]);
    print_test("La pila no esta vacia", !pila_esta_vacia(pila));

    pila_destruir(pila);
}


static void prueba_apilar_null(void){
    printf("\nINICIO DE PRUEBAS SE PUEDE APILAR NULL\n");

    pila_t *pila = pila_crear();

    print_test("Apilar NULL es valido", pila_apilar(pila, NULL));
    print_test("La pila no esta vacia", !pila_esta_vacia(pila));
    print_test("El elemento del tope es el NULL", pila_ver_tope(pila) == NULL);
    print_test("El elemento desapilado es el NULL", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
}


static void prueba_desapilar_elementos(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN DESAPILAR ELEMENTOS\n");

    pila_t *pila = pila_crear();
    int valores[3];

    bool ok = true;
    for (int i = 0; i < 3; i++) {
        if (!pila_apilar(pila, &valores[i])) ok = false;
    }
    print_test("Pila creada y elementos de prueba apilados", ok);

    print_test("Se desapilo elemento 2", pila_desapilar(pila) == &valores[2]);
    print_test("El tope es el elemento 1", pila_ver_tope(pila) == &valores[1]);
    print_test("Se desapilo elemento 1", pila_desapilar(pila) == &valores[1]);
    print_test("El tope es el elemento 0", pila_ver_tope(pila) == &valores[0]);
    print_test("La pila no esta vacia", !pila_esta_vacia(pila));
    print_test("Se desapilo elemento 0", pila_desapilar(pila) == &valores[0]);
    print_test("La pila esta vacia", pila_esta_vacia(pila));
    print_test("Desapilar elementos devuelve NULL", pila_desapilar(pila) == NULL);
    print_test("Ver el tope de la pila devuelve NULL", pila_ver_tope(pila) == NULL);

    pila_destruir(pila);
}


static void prueba_volumen(void){
    printf("\nINICIO DE PRUEBAS DE VOLUMEN DE ELEMENTOS\n");

    pila_t *pila = pila_crear();
    int size_valores = 1000;
    int valores[size_valores];
    bool ok = true;

    printf("Apilando 1000 elementos..\n");
    for (int i = 0; i < size_valores; i++){
        if (!pila_apilar(pila, &valores[i])) {
            ok = false;
        }
    }
    print_test("El apilado de 1000 elementos fue correcto", ok);
    print_test("El tope de los 1000 elementos coincide", pila_ver_tope(pila) == &valores[size_valores-1]);
    
    ok = true;
    printf("Desapilando 1000 elementos..\n");
    for (int j = size_valores; j > 0; j--) {
        if (pila_desapilar(pila) != &valores[j-1]) {
            ok = false;
        }
    }

    print_test("Los 1000 elementos desapilados son los correctos", ok);
    print_test("La pila esta vacia", pila_esta_vacia(pila));

    pila_destruir(pila);
}


void pruebas_pila_estudiante() {
    prueba_crear_destruir_pila();
    prueba_pila_vacia();
    prueba_apilar_elementos();
    prueba_apilar_null();
    prueba_desapilar_elementos();
    prueba_volumen();
}

/*
 * Función main() que llama a la función de pruebas.
*/

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
