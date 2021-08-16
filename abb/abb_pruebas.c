/*
 * prueba_abb.c
 * Pruebas para el tipo de dato abstracto Tabla de abb
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */

#include "abb.h"
#include "testing.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_insertar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    // Inserta 1 valor y luego lo borra 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    // Inserta otros 2 valores y no los borra (se destruyen con el abb) 
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    // Inserta 2 valores y luego los reemplaza 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir()
{
    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    // Pide memoria para 4 valores 
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    // Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    // Se destruye el abb (se debe liberar lo que quedó dentro) 
    abb_destruir(abb);
}

static void prueba_abb_borrar()
{
    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    char *clave4 = "vaca2", *valor4 = "mu2";
    char *clave5 = "gato3", *valor5 = "miau3";
    char *clave6 = "abeja", *valor6 = "bzz";

    printf("Pruebas borrar sin hijos\n");
    abb_t* abb = abb_crear(strcmp, NULL);
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);

    printf("Pruebas borrar con 1 hijo\n");
    abb = abb_crear(strcmp, NULL);
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, valor4));
    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, valor6));
    print_test("Prueba abb borrar clave2 con solo hijo derecho, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave3 con solo hijo izquierdo, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);
    abb_destruir(abb);

    printf("Pruebas borrar con 2 hijos\n");
    abb = abb_crear(strcmp, NULL);
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, valor5));
    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, valor6));
    print_test("Prueba abb borrar clave2 con 2 hijos, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb la cantidad de elementos es 4", abb_cantidad(abb) == 4);
    print_test("Prueba abb borrar clave1 (raiz) con 2 hijos, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);
    abb_destruir(abb);

}

static void prueba_abb_clave_vacia()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    // Inserta 1 valor y luego lo borra 
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_volumen(size_t largo, bool debug)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    // Inserta 'largo' parejas en el abb 
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }

    for (size_t i = 0; i < largo; i++) {
        size_t n_random = rand() % (largo - 1);
        unsigned aux = *valores[i];
        *valores[i] = *valores[n_random];
        *valores[n_random] = aux;
    }

    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    // Verifica que devuelva los valores correctos 
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    // Verifica que borre y devuelva los valores correctos 
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb borrar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    // Destruye el abb y crea uno nuevo que sí libera 
    abb_destruir(abb);
    abb = abb_crear(strcmp, free);

    // Inserta 'largo' parejas en el abb 
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    // Destruye el abb - debería liberar los enteros 
    abb_destruir(abb);

}

// // Funciones auxiliares para funcion visitar

// Pre: el void* extra es un puntero a un entero
// Post: devuelve true sin importar los datos, devuelve la suma en el extra
// Deja todos los elementos como estan (solo sirve para datos enteros)
bool visitar_suma_total(const char* clave, void* dato, void* extra) {
    *(int*) extra = *(int*) extra + *(int*) dato;
    return true;
}

// Pre: el void* extra es un puntero a arreglo con extra[0]=suma_total, extra[1]=n_corte
// Post: si n_corte es igual a 2, devuelve false, en caso contrario devuelve true. guarda la suma de los iterados en el extra[0]
bool visitar_suma_primeros_2(const char* clave, void* dato, void* extra) {
    int* arreglo = (int*) extra;
    arreglo[0] = arreglo[0] + *(int*) dato;
    *(int*) dato = 0;
    arreglo[1]++;
    if (arreglo[1] == 2) return false;
    return true;
}

static void prueba_iterar_con_condicion_corte(void) {
    
    abb_t* abb = abb_crear(strcmp, NULL);
    int dato[] = {0, 0};
    char* claves[5] = {"gato", "perro", "oveja", "vaca"}; // Orden: gato, oveja, perro, vaca
    int enteros[] = {1, 2, 3, 4};
    
    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!abb_guardar(abb, claves[i], &enteros[i])) {
            ok = false;
        }
    }

    print_test("Se creo abb con elementos a iterar", ok);
    abb_in_order(abb, visitar_suma_primeros_2, &dato);
    print_test("El dato de la funcion visitar es correcto (itero hasta la posicion correcta)", dato[0] == 4);

    // Verificamos que no se haya aplicado la funcion visitar a todos los elementos
    // Si el iterador ignora que visitar = false, todos valdrían 0
    ok = true;
    if (*(int*) abb_obtener(abb, "gato") != 0) ok = false;
    if (*(int*) abb_obtener(abb, "oveja") != 0) ok = false;
    if (*(int*) abb_obtener(abb, "perro") == 0) ok = false;
    if (*(int*) abb_obtener(abb, "vaca") == 0) ok = false;

    print_test("El iterador interno considera correctamente el booleano para dejar de iterar", ok);

    abb_destruir(abb);
}

static void prueba_iterar_sin_condicion_corte(void) {

    abb_t* abb = abb_crear(strcmp, NULL);
    int dato = 0;
    char* claves[5] = {"gato", "perro", "oveja", "vaca"}; // Orden: gato, oveja, perro, vaca
    int enteros[] = {1, 2, 3, 4};
    
    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!abb_guardar(abb, claves[i], &enteros[i])) {
            ok = false;
        }
    }

    print_test("Se creo abb con elementos a iterar", ok);
    abb_in_order(abb, visitar_suma_total, &dato);
    print_test("Itero todo el abb correctamente y aplico la funcion visitar", dato == 10);

    abb_destruir(abb);
}

static void prueba_abb_iterar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca", "vaca2", "gato3"};
    char *valores[] = {"guau", "miau", "mu", "mu2", "miau3"};
    size_t i_ordenados[] = {1, 4, 0, 2, 3};

    // Inserta 3 valores 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, claves[3], valores[3]));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, claves[4], valores[4]));

    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba crear iterador", iter);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    size_t i = 0;
    while (!abb_iter_in_al_final(iter)) {
        if (i > 4) break;
        char* clave = (char*) abb_iter_in_ver_actual(iter);
        print_test("Prueba abb iterador ver actual es la clave correcta", strcmp(clave, claves[i_ordenados[i++]]) == 0);
        abb_iter_in_avanzar(iter);
    }

    print_test("Se iteraron todos los elementos", i == 5);

    // Vuelve a tratar de avanzar, por las dudas 
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];
    for (size_t i = 0; i < largo; i++) valores[i] = i;

    for (size_t i = 0; i < largo; i++) {
        size_t n_random = rand() % (largo - 1);
        size_t aux = valores[i];
        valores[i] = valores[n_random];
        valores[n_random] = aux;
    }

    // Inserta 'largo' parejas en el abb 
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08ld", valores[i]);
        ok = abb_guardar(abb, claves[i], &valores[i]);
        if (!ok) break;
    }

    print_test("Se guardo correctamente todo el volumen de elementos", ok && abb_cantidad(abb) == largo);
    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_abb_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */

    printf("\nPRUEBAS CREAR ABB VACIO\n");
    prueba_crear_abb_vacio();

    printf("\nPRUEBAS ITERAR ABB VACIO\n");
    prueba_iterar_abb_vacio();

    printf("\nPRUEBAS ABB INSERTAR\n");
    prueba_abb_insertar();

    printf("\nPRUEBAS ABB REEMPLAZAR\n");
    prueba_abb_reemplazar();

    printf("\nPRUEBAS ABB REEMPLAZAR CON DESTRUIR\n");
    prueba_abb_reemplazar_con_destruir();

    printf("\nPRUEBAS ABB BORRAR\n");
    prueba_abb_borrar();

    printf("\nPRUEBAS ABB CLAVE VACIA\n");
    prueba_abb_clave_vacia();

    printf("\nPRUEBAS ABB VALOR NULL\n");
    prueba_abb_valor_null();

    printf("\nPRUEBAS ABB VOLUMEN\n");
    prueba_abb_volumen(50, true);

    printf("\nPRUEBAS ABB ITERADOR INTERNO CON CONDICION DE CORTE\n");
    prueba_iterar_con_condicion_corte();

    printf("\nPRUEBAS ABB ITERADOR INTERNO SIN CONDICION DE CORTE\n");
    prueba_iterar_sin_condicion_corte();

    printf("\nPRUEBAS ABB ITERAROR EXTERNO\n");
    prueba_abb_iterar();

    printf("\nPRUEBAS ABB ITERADOR EXTERNO VOLUMEN\n");
    prueba_abb_iterar_volumen(5000);
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif