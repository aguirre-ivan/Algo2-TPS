#include "pila.h"
#include "lista.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* *****************************************************************
 *                      PRUEBAS LISTA
 * *****************************************************************/

static void prueba_crear_destruir_lista(void) {
    printf("\nINICIO PRUEBA SE PUEDE CREAR Y DESTRUIR LISTA (VACIA)\n");
    lista_t *lista = lista_crear();

    print_test("La lista fue creada", lista != NULL);

    lista_destruir(lista, NULL);
    printf("Lista destruida\n");
}

static void prueba_funcion_destruir_vacia_otra_funcion(void) {
    printf("\nINICIO PRUEBAS DESTRUIR LISTA VACIA PASANDO FUNCION\n");
    lista_t *lista = lista_crear();

    print_test("La lista fue creada", lista != NULL);

    lista_destruir(lista, wrapper_pila_destruir);
    printf("Lista destruida\n");
}

static void prueba_lista_vacia(void) {
    printf("\nINICIO DE PRUEBAS CON LISTA VACIA\n");

    lista_t* lista = lista_crear();
    
    print_test("La lista fue creada", lista != NULL);
    print_test("Lista esta vacia devuelve TRUE", lista_esta_vacia(lista));
    print_test("El largo de la lista es 0", lista_largo(lista) == 0);
    print_test("Ver primero de la lista devuelve NULL", lista_ver_primero(lista) == NULL);
    print_test("Ver ultimo de la lista devuelve NULL", lista_ver_primero(lista) == NULL);
    print_test("Lista borrar primero devuelve NULL", lista_borrar_primero(lista) == NULL);

    lista_destruir(lista, NULL);
}

static void prueba_insertar_elementos_primero(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN INSERTAR ELEMENTOS PRIMEROS\n");

    lista_t* lista = lista_crear();
    int enteros[3];

    print_test("Insertar primero entero 0 es True", lista_insertar_primero(lista, &enteros[0]));
    print_test("El primero de la lista es entero 0", lista_ver_primero(lista) == &enteros[0]);
    print_test("Insertar primero entero 1 es True", lista_insertar_primero(lista, &enteros[1]));
    print_test("El primero de la lista es entero 1", lista_ver_primero(lista) == &enteros[1]);
    print_test("Insertar primero entero 2 es True", lista_insertar_primero(lista, &enteros[2]));
    print_test("El primero de la lista es entero 2", lista_ver_primero(lista) == &enteros[2]);
    print_test("El ultimo de la lista es entero 0", lista_ver_ultimo(lista) == &enteros[0]);
    print_test("El largo de la lista es 3", lista_largo(lista) == 3);
    print_test("La lista no esta vacia", !lista_esta_vacia(lista));

    lista_destruir(lista, NULL);
}

static void prueba_insertar_elementos_ultimo(void){
    printf("\nINICIO DE PRUEBAS SE PUEDEN INSERTAR ELEMENTOS ULTIMOS\n");

    lista_t* lista = lista_crear();
    char caract[3];
    
    print_test("Insertar ultimo char 0 es True", lista_insertar_ultimo(lista, &caract[0]));
    print_test("El ultimo de la lista es char 0", lista_ver_ultimo(lista) == &caract[0]);
    print_test("Insertar ultimo char 1 es True", lista_insertar_ultimo(lista, &caract[1]));
    print_test("El ultimo de la lista es char 1", lista_ver_ultimo(lista) == &caract[1]);
    print_test("Insertar ultimo char 2 es True", lista_insertar_ultimo(lista, &caract[2]));
    print_test("El ultimo de la lista es char 2", lista_ver_ultimo(lista) == &caract[2]);
    print_test("El primero de la lista es char 0", lista_ver_primero(lista) == &caract[0]);
    print_test("El largo de la lista es 3", lista_largo(lista) == 3);
    print_test("La lista no esta vacia", !lista_esta_vacia(lista));

    lista_destruir(lista, NULL);
}

static void prueba_insertar_null(void){
    printf("\nINICIO DE PRUEBAS SE PUEDE INSERTAR ELEMENTO NULL\n");
    lista_t* lista = lista_crear();

    print_test("Insertar primero NULL es valido", lista_insertar_primero(lista, NULL));
    print_test("El primero de la lista es NULL", lista_ver_primero(lista) == NULL);
    print_test("El largo de la lista es 1", lista_largo(lista) == 1);
    print_test("Insertar ultimo NULL es valido", lista_insertar_ultimo(lista, NULL));
    print_test("El ultimo de la lista es NULL", lista_ver_ultimo(lista) == NULL);
    print_test("El largo de la lista es 2", lista_largo(lista) == 2);
    print_test("La lista no esta vacia", !lista_esta_vacia(lista));

    lista_destruir(lista, NULL);
}

static void prueba_borrar_primero(void){
    printf("\nINICIO DE PRUEBAS BORRAR PRIMEROS\n");

    lista_t *lista = lista_crear();
    int enteros[3];

    for (int i = 0; i < 3; i++) {
        lista_insertar_ultimo(lista, &enteros[i]);
    }
    
    print_test("Se elimino el elemento 0", lista_borrar_primero(lista) == &enteros[0]);
    print_test("Se elimino el elemento 1", lista_borrar_primero(lista) == &enteros[1]);
    print_test("Se elimino el elemento 2", lista_borrar_primero(lista) == &enteros[2]);

    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("Ver el primero de la lista devuelve NULL", lista_ver_primero(lista) == NULL);
    print_test("Eliminar primero de la lista devuelve NULL", lista_borrar_primero(lista) == NULL);

    lista_destruir(lista, NULL);
}

static void prueba_volumen(void){
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

    lista_t *lista = lista_crear();
    int size_enteros = 1000;
    int enteros[size_enteros];
    bool ok = true;

    printf("Insertando 1000 elementos..\n");
    for (int i = 0; i < size_enteros; i++){
        if (!lista_insertar_ultimo(lista, &enteros[i])) {
            ok = false;
        }
    }
    print_test("El agregado de 1000 elementos fue correcto", ok);
    print_test("El primero de la lista es 0", lista_ver_primero(lista) == &enteros[0]);
    print_test("El ultimo de la lista es 999", lista_ver_ultimo(lista) == &enteros[999]);
    print_test("El largo de la lista es 1000", lista_largo(lista) == 1000);
    
    ok = true;
    printf("Eliminando 1000 elementos..\n");
    for (int j = 0; j < size_enteros; j++) {
        if (lista_borrar_primero(lista) != &enteros[j]) {
            ok = false;
        }
    }
    print_test("Los elementos eliminados fueron correctos", ok);
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("El largo de la lista es 0", lista_largo(lista) == 0);
    print_test("Ver el primero de la lista devuelve NULL", lista_ver_primero(lista) == NULL);
    print_test("Ver el ultimo de la lista devuelve NULL", lista_ver_ultimo(lista) == NULL);
    
    lista_destruir(lista, NULL);
}

static void prueba_funcion_destruir_free(void){
    printf("\nINICIO DE PRUEBAS DESTRUIR LISTA (CON FUNCION FREE)\n");

    lista_t *lista = lista_crear();

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
        printf("Insertando elementos de memoria dinamica a destruir con free\n");
        print_test("Se agrego el primer elemento", lista_insertar_ultimo(lista, entero1));
        print_test("Se agrego el segundo elemento", lista_insertar_ultimo(lista, entero2));
        print_test("Se agrego el tercer elemento", lista_insertar_ultimo(lista, string1));
        print_test("Se agrego el cuarto elemento", lista_insertar_ultimo(lista, string2));
        
    }
    lista_destruir(lista, free);
    printf("La lista fue destruida\n");
}

static void prueba_funcion_destruir_otra_funcion(void) {
    printf("\nINICIO DE PRUEBAS DESTRUIR LISTA (CON OTRA FUNCION)\n");

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

    lista_t* lista = lista_crear();
    print_test("Insertar pila1", lista_insertar_ultimo(lista, pila1));
    print_test("Insertar pila2", lista_insertar_ultimo(lista, pila2));
    print_test("Insertar pila3", lista_insertar_ultimo(lista, pila3));
    lista_destruir(lista, wrapper_pila_destruir);
    printf("Lista con pilas destruida\n");
}

/* *****************************************************************
 *                      PRUEBAS ITERADOR INTERNO
 * *****************************************************************/

// Funciones auxiliares para funcion visitar

// Pre: el void* extra es un puntero a un entero
// Post: devuelve true sin importar los datos, devuelve la suma en el extra
// Deja todos los elementos como estan (solo sirve para una lista de enteros)
bool visitar_suma_total(void* dato, void* extra) {
    *(int*) extra = *(int*) extra + *(int*) dato;
    return true;
}

// Pre: el void* extra es un puntero a un entero
// Post: si dato es igual a 2, devuelve false, en caso contrario devuelve true. guarda la suma de los iterados en el extra
bool visitar_suma_mitad(void* dato, void* extra) {
    *(int*) extra = *(int*) extra + *(int*) dato;
    return !(*(int*) dato == 2);
}

static void prueba_iterar_lista_vacia(void) {
    printf("\nINICIO DE PRUEBAS ITERAR LISTA VACIA\n");
    lista_t* lista = lista_crear();

    print_test("La lista fue creada", lista!=NULL);
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    lista_iterar(lista, visitar_suma_total, NULL);
    lista_iterar(lista, visitar_suma_mitad, NULL);
    print_test("La lista sigue vacia luego de iterarla con funciones con y sin corte", lista_esta_vacia(lista));

    lista_destruir(lista, NULL);
}

static void prueba_iterar_lista_sin_visitar(void) {
    printf("\nINICIO DE PRUEBAS ITERAR LISTA (SIN FUNCION)\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) {
            ok = false;
        }
    }

    print_test("Creando lista de prueba", ok);
    lista_iterar(lista, NULL, NULL);

    ok = true;
    for (int i = 0; i < 4; i++) {
        if (lista_borrar_primero(lista) != &enteros[i]) {
            ok = false;
        }
    }
    print_test("Se le aplico funcion iterar a la lista y los elementos se mantuvieron igual", ok);

    lista_destruir(lista, NULL);
}

static void prueba_iterar_con_condicion_corte(void) {
    printf("\nINICIO DE PRUEBAS ITERAR LISTA (CON FUNCION CON CORTE)\n");
    lista_t* lista = lista_crear();
    int dato = 0;
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) {
            ok = false;
        }
    }

    print_test("Creando lista para suma {1, 2, 3, 4}", ok);
    lista_iterar(lista, visitar_suma_mitad, &dato);
    printf("La lista se corta cuando un elemento es 2\n");
    print_test("Itero la lista y la suma total de la mitad es 3", dato == 3);

    ok = true;
    for (int i = 0; i < 4; i++) {
        if (lista_borrar_primero(lista) != &enteros[i]) {
            ok = false;
        }
    }
    print_test("Luego de iterar la lista los elementos se mantuvieron igual", ok);

    lista_destruir(lista, NULL);

}

static void prueba_iterar_sin_condicion_corte(void) {
    printf("\nINICIO DE PRUEBAS ITERAR LISTA (CON FUNCION SIN CORTE)\n");
    lista_t* lista = lista_crear();
    int dato = 0;
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) {
            ok = false;
        }
    }

    print_test("Creando lista para suma {1, 2, 3, 4}", ok);
    lista_iterar(lista, visitar_suma_total, &dato);
    print_test("Itero la lista y la suma total es 10", dato == 10);

    ok = true;
    for (int i = 0; i < 4; i++) {
        if (lista_borrar_primero(lista) != &enteros[i]) {
            ok = false;
        }
    }
    print_test("Luego de iterar la lista los elementos se mantuvieron igual", ok);

    lista_destruir(lista, NULL);
}


/* *****************************************************************
 *                      PRUEBAS ITERADOR EXTERNO
 * *****************************************************************/

static void prueba_crear_destruir_iterador(void) {
    printf("\nINICIO DE PRUEBAS CREAR Y DESTRUIR ITERADOR\n");
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);

    print_test("Iterador creado", iter!=NULL);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("Iterador destruido\n");
}

static void prueba_iterar_hasta_final(void) {
    printf("\nINICIO DE PRUEBAS ITERAR HASTA EL FINAL\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    print_test("Creando lista de prueba", ok);

    ok = true;
    lista_iter_t* iter = lista_iter_crear(lista);
    for (int i = 0; i < 4; i++) {
        if (lista_iter_ver_actual(iter) != &enteros[i]) ok = false;
        lista_iter_avanzar(iter);
    }
    print_test("Se recorre la lista manualmente y los elementos coinciden", ok);
    print_test("Se itera hasta el final, el iterador esta al final", lista_iter_al_final(iter));
    lista_iter_borrar(iter);
    print_test("Al intentar borrar elemento, la lista sigue igual", lista_largo(lista) == 4);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_ver_elemento(void) {
    printf("\nINICIO DE PRUEBAS VER ELEMENTO ACTUAL DEL ITERADOR\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    print_test("Creando lista de prueba", ok);
    lista_iter_t* iter = lista_iter_crear(lista);

    print_test("El iterador apunta al primer elemento", lista_iter_ver_actual(iter) == &enteros[0]);
    lista_iter_avanzar(iter);
    print_test("El iterador apunta al segundo elemento", lista_iter_ver_actual(iter) == &enteros[1]);
    lista_iter_avanzar(iter);
    print_test("El iterador apunta al tercer elemento", lista_iter_ver_actual(iter) == &enteros[2]);
    lista_iter_avanzar(iter);
    print_test("El iterador apunta al cuarto elemento", lista_iter_ver_actual(iter) == &enteros[3]);
    lista_iter_avanzar(iter);
    print_test("El iterador esta al final", lista_iter_al_final(iter));
    
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_insertar_final(void) {
    printf("\nINICIO DE PRUEBAS INSERTAR CON ITERADOR AL FINAL\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};
    int dato = 5;

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    print_test("Creando lista de prueba", ok);

    lista_iter_t* iter = lista_iter_crear(lista);
    while (!lista_iter_al_final(iter)) {
        if (!lista_iter_avanzar(iter)) break;
    }
    print_test("Se avanzo hasta el final", lista_iter_al_final(iter));

    print_test("Insertando elemento", lista_iter_insertar(iter, &dato));
    print_test("El largo de la lista aumento uno", lista_largo(lista) == 5);
    print_test("El ultimo de la lista es el elemento insertado", lista_ver_ultimo(lista) == &dato);
    print_test("El iterador apunta al elemento insertado", lista_iter_ver_actual(iter) == &dato);
    lista_iter_avanzar(iter);
    print_test("El iterador avanzado esta al final", lista_iter_al_final(iter));
    print_test("El primer elemento se mantuvo igual", lista_ver_primero(lista) == &enteros[0]);

    lista_destruir(lista, NULL);
    lista_iter_destruir(iter);
}

static void prueba_insertar_principio(void) {
    printf("\nINICIO DE PRUEBAS INSERTAR CON ITERADOR AL PRINCIPIO\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};
    int dato = 0;

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    print_test("Creando lista de prueba", ok);
    lista_iter_t* iter = lista_iter_crear(lista);
    
    print_test("Insertando elemento", lista_iter_insertar(iter, &dato));
    print_test("El largo de la lista aumento uno", lista_largo(lista) == 5);
    print_test("El primero de la lista es el elemento insertado", lista_ver_primero(lista) == &dato);
    print_test("El iterador apunta al elemento insertado", lista_iter_ver_actual(iter) == &dato);
    lista_iter_avanzar(iter);
    print_test("El elemento siguiente al insertado es el esperado", lista_iter_ver_actual(iter) == &enteros[0]);
    print_test("El ultimo elemento se mantuvo igual", lista_ver_ultimo(lista) == &enteros[3]);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_insertar_medio(void) {
    printf("\nINICIO DE PRUEBAS INSERTAR CON ITERADOR EN EL MEDIO\n");

    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 4, 5};
    int dato = 3;

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    print_test("Creando lista de prueba", ok);
    lista_iter_t* iter = lista_iter_crear(lista);

    ok = true;
    for (int i = 0; i < 2; i++) {
        ok = lista_iter_avanzar(iter);
        if (!ok) break;
    }
    print_test("Se avanzo hasta la posicion deseada", ok);
    print_test("Insertando elemento", lista_iter_insertar(iter, &dato));
    print_test("El largo de la lista aumento uno", lista_largo(lista) == 5);
    print_test("El iterador apunta al elemento insertado", lista_iter_ver_actual(iter) == &dato);
    lista_iter_avanzar(iter);
    print_test("El elemento siguiente al insertado es el esperado", lista_iter_ver_actual(iter) == &enteros[2]);
    print_test("El primer y ultimo elemento se mantuvieron igual", lista_ver_primero(lista) == &enteros[0] && lista_ver_ultimo(lista) == &enteros[3]);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_remover_final(void) {
    printf("\nINICIO DE PRUEBAS REMOVER CON ITERADOR ULTIMO ELEMENTO\n");

    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    if (lista_largo(lista) != 4) ok = false;
    print_test("Creando lista de prueba", ok);

    lista_iter_t* iter = lista_iter_crear(lista);
    ok = true;
    for (int i = 0; i < 3; i++) {
        ok = lista_iter_avanzar(iter);
        if (!ok) break;
    }
    print_test("Se creo el iterador y se avanzo hasta el ultimo", ok);
    print_test("El iterador apunta al ultimo elemento", lista_iter_ver_actual(iter) == lista_ver_ultimo(lista));

    print_test("El elemento borrado es el correcto (ultimo)", lista_iter_borrar(iter) == &enteros[3]);
    print_test("El nuevo ultimo de la lista es correcto", lista_ver_ultimo(lista) == &enteros[2]);
    print_test("El iterador apunta al siguiente elemento (queda fuera)", lista_iter_al_final(iter));
    print_test("El primer elemento se mantuvo correctamente", lista_ver_primero(lista) == &enteros[0]);
    print_test("El largo de la lista es el correcto", lista_largo(lista) == 3);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_remover_principio(void) {
    printf("\nINICIO DE PRUEBAS REMOVER CON ITERADOR PRIMER ELEMENTO\n");

    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    if (lista_largo(lista) != 4) ok = false;
    print_test("Creando lista de prueba", ok);
    lista_iter_t* iter = lista_iter_crear(lista);
    print_test("Se iterador creado apunta al primero", lista_iter_ver_actual(iter) == lista_ver_primero(lista));

    print_test("El elemento borrado es el correcto (primero)", lista_iter_borrar(iter) == &enteros[0]);
    print_test("El largo de la lista es el correcto", lista_largo(lista) == 3);
    print_test("El primero de la lista es correcto", lista_ver_primero(lista) == &enteros[1]);
    print_test("El iterador apunta al siguiente elemento(al nuevo primero)", lista_iter_ver_actual(iter) == &enteros[1]);
    print_test("El ultimo de la lista se mantuvo correctamente", lista_ver_ultimo(lista) == &enteros[3]);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_remover_medio(void) {
    printf("\nINICIO DE PRUEBAS REMOVER CON ITERADOR ELEMENTO DEL MEDIO\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    if (lista_largo(lista) != 4) ok = false;
    print_test("Creando lista de prueba", ok);

    lista_iter_t* iter = lista_iter_crear(lista);
    ok = true;
    for (int i = 0; i < 2; i++) {
        ok = lista_iter_avanzar(iter);
        if (!ok) break;
    }
    if (lista_iter_ver_actual(iter) != &enteros[2]) ok = false;
    print_test("Se avanzo correctamente hasta la posicion 2", ok);

    int* dato = lista_iter_borrar(iter);
    
    print_test("El elemento borrado es el valor2", dato == &enteros[2]);
    print_test("El largo de la lista disminuyo uno", lista_largo(lista) == 3);
    print_test("El iterador apunta al elemento que estaba siguiente", lista_iter_ver_actual(iter) == &enteros[3]);
    print_test("El ultimo y primero de la lista se mantuvieron igual", lista_ver_primero(lista) == &enteros[0] && lista_ver_ultimo(lista) == &enteros[3]);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void pruebas_remover_lista_entera(void) { 
    printf("\nINICIO DE PRUEBAS REMOVER CON ITERADOR LISTA ENTERA\n");
    lista_t* lista = lista_crear();
    int enteros[] = {1, 2, 3, 4};

    bool ok = true;
    for (int i = 0; i < 4; i++) {
        if (!lista_insertar_ultimo(lista, &enteros[i])) ok = false;
        if (!ok) break;
    }
    if (lista_largo(lista) != 4) ok = false;
    print_test("Creando lista de prueba", ok);
    lista_iter_t* iter = lista_iter_crear(lista);

    print_test("Creo un iterador (posicionado en el primer elemento)", lista_iter_ver_actual(iter) == lista_ver_primero(lista));

    while (!lista_iter_al_final(iter)) {
        lista_iter_borrar(iter);
    }
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("El largo de la lista es 0", lista_largo(lista) == 0);
    print_test("El iterador apunta a NULL", lista_iter_ver_actual(iter) == NULL);
    print_test("El iterador quedo al final", lista_iter_al_final(iter));

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante() {
    //Pruebas primitivas basicas lista
    printf("\n---------------------------------\n");
    printf("PRUEBAS PRIMITIVAS LISTA");
    printf("\n---------------------------------\n");
    prueba_crear_destruir_lista();
    prueba_funcion_destruir_vacia_otra_funcion();
    prueba_lista_vacia();
    prueba_insertar_elementos_primero();
    prueba_insertar_elementos_ultimo();
    prueba_insertar_null();
    prueba_borrar_primero();
    prueba_volumen();
    prueba_funcion_destruir_free();
    prueba_funcion_destruir_otra_funcion();
    
    //Pruebas iterador interno
    printf("\n---------------------------------\n");
    printf("PRUEBAS ITERADOR INTERNO");
    printf("\n---------------------------------\n");
    prueba_iterar_lista_vacia();
    prueba_iterar_lista_sin_visitar();
    prueba_iterar_con_condicion_corte();
    prueba_iterar_sin_condicion_corte();

    //Pruebas iterador externo
    printf("\n---------------------------------\n");
    printf("PRUEBAS ITERADOR EXTERNO");
    printf("\n---------------------------------\n");
    prueba_crear_destruir_iterador();
    prueba_iterar_hasta_final();
    prueba_ver_elemento();
    prueba_insertar_final();
    prueba_insertar_principio();
    prueba_insertar_medio();
    prueba_remover_final();
    prueba_remover_principio();
    prueba_remover_medio();
    pruebas_remover_lista_entera();
}

#ifndef CORRECTOR

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;
}

#endif
