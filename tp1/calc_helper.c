#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* *****************************************************************
 *                FUNCIONES INTERNAS DE CALC HELPER.C
 * *****************************************************************/

// Convierte una cadena a entero, indicando si se pudo.
//
// Args:
//    tok: la cadena a convertir
//    dest: dónde guardar el valor convertido
//
// Returns:
//    verdadero si la cadena era en su totalidad un entero válido.
//
static bool parse_num(const char *tok, calc_num *dest) {
    char *end;
    *dest = strtol(tok, &end, 10);
    return *end == '\0';
}

//
// Convierte palabra a struct calc_token (número, operador, o paréntesis).
//
// Args:
//    tok: la palabra a convertir
//    dest: dónde guardar el token extraído
//
// Returns:
//    verdadero si la palabra formaba un token válido, falso en caso contrario
//
bool calc_parse(const char *tok, struct calc_token *parsed) {
    if (parse_num(tok, &parsed->value)) {
        parsed->type = TOK_NUM;
        return true;
    } else {
        parsed->type = TOK_OPER;
    }

    if (strlen(tok) == 1) {
        char op = tok[0];
        if (op == '+') {
            parsed->oper.op = OP_ADD;
            parsed->oper.arity = 2;
            parsed->oper.precedence = 2;
            parsed->oper.associativity = ASSOC_RIGHT;
        }
        else if (op == '-') {
            parsed->oper.op = OP_SUB;
            parsed->oper.arity = 2;
            parsed->oper.precedence = 2;
            parsed->oper.associativity = ASSOC_RIGHT;
        }
        else if (op == '*') {
            parsed->oper.op = OP_MUL;
            parsed->oper.arity = 2;
            parsed->oper.precedence = 3;
            parsed->oper.associativity = ASSOC_RIGHT;
        }
        else if (op == '/') {
            parsed->oper.op = OP_DIV;
            parsed->oper.arity = 2;
            parsed->oper.precedence = 3;
            parsed->oper.associativity = ASSOC_RIGHT;
        }
        else if (op == '^') {
            parsed->oper.op = OP_POW;
            parsed->oper.arity = 2;
            parsed->oper.precedence = 4;
            parsed->oper.associativity = ASSOC_LEFT;
        }
        else if (op == '?') {
            parsed->oper.op = OP_TERN;
            parsed->oper.arity = 3;
        }
        else if (op == '(') {
            parsed->type = TOK_LPAREN;
        }
        else if (op == ')') {
            parsed->type = TOK_RPAREN;
        }
        else {
            return false;
        }
        parsed->oper.char_op = op;
    }
    else if (strcmp(tok, "log") == 0) {
        parsed->oper.op = OP_LOG;
        parsed->oper.arity = 2;
    }
    else if (strcmp(tok, "sqrt") == 0) {
        parsed->oper.op = OP_RAIZ;
        parsed->oper.arity = 1;
    }
    else {
        return false;
    }
    return true;
}

/* *****************************************************************
 *                       PRIMITIVAS PILA NUM
 * *****************************************************************/

pilanum_t *pilanum_crear(void) {
    return pila_crear();
}

void apilar_num(pilanum_t *pila, calc_num num) {
    calc_num *dyn;
    // Por comodidad, queremos que apilar_num() sea void. Añadimos
    // sendos asserts para que, si ocurriese que malloc() falla, fuera
    // obvio qué está pasando.
    assert((dyn = malloc(sizeof(calc_num))));
    *dyn = num;
    assert(pila_apilar(pila, dyn));
}


bool desapilar_num(pilanum_t *pila, calc_num *num) {
    if (pila_esta_vacia(pila)) {
        return false;
    }
    calc_num *dyn = pila_desapilar(pila);
    *num = *dyn;
    free(dyn);
    return true;
}

bool pilanum_esta_vacia(pilanum_t* pila) {
    return pila_esta_vacia(pila);
}

void pilanum_destruir(pilanum_t *pila) {
    while (!pila_esta_vacia(pila)) {
        void *elem = pila_desapilar(pila);
        free(elem);
    }
    pila_destruir(pila);
}

/* *****************************************************************
 *                       PRIMITIVAS COLA TOK
 * *****************************************************************/

colatok_t* colatok_crear(void) {
    return cola_crear();
}

bool encolar_token(colatok_t* cola, calc_token* token) {
    return cola_encolar(cola, (void*) token);
}

void colatok_destruir(colatok_t* cola) {
    //En infix siempre destruimos una cola vacia.
    cola_destruir(cola, NULL);
}

calc_token* desencolar_token(colatok_t* cola) {
    return (calc_token*) cola_desencolar(cola);
}

bool colatok_esta_vacia(colatok_t* cola) {
    return cola_esta_vacia(cola);
}

/* *****************************************************************
 *                       PRIMITIVAS PILA TOK
 * *****************************************************************/

pilatok_t* pilatok_crear(void) {
    return pilanum_crear();
}

void pilatok_destruir(pilatok_t* pila) {
    pilanum_destruir(pila);
}

bool apilar_token(pilatok_t* pila, calc_token* token) {
    return pila_apilar(pila, (void*) token);
}

calc_token* desapilar_token(pilatok_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;
    return (calc_token*) pila_desapilar(pila);
}

bool pilatok_esta_vacia(pilatok_t* pila) {
    return pila_esta_vacia(pila);
}

calc_token* pilatok_ver_tope(pilatok_t* pila){
    return (calc_token*) pila_ver_tope(pila);
}


/* *****************************************************************
 *                 FUNCIONES DC_SPLIT e INFIX_SPLIT
 * *****************************************************************/

//
// Implementaciones de dc_split e infix_split.
//
// En ambos casos el argumento es una línea de texto, y el valor de retorno
// un arreglo dinámico de cadenas dinámicas. Si la cadena estaba vacía o no
// tenía tokens, se devuelve un arreglo de longitud cero: {NULL}.
//

char **dc_split(const char *linea) {
    const char *delim = " \n";
    char *parse = strdup(linea);  // No modificar línea, que es const.
    char *ptr, *tok = strtok_r(parse, delim, &ptr);
    char **strv = calloc(strlen(linea) + 1, sizeof(char *));  // (¹)

    // Separa por espacios (o múltiples espacios), e ignorando saltos de línea.
    for (size_t i = 0; tok != NULL; tok = strtok_r(NULL, delim, &ptr), i++) {
        strv[i] = strdup(tok);
    }

    free(parse);
    return strv;
}

char **infix_split(const char *linea) {
    size_t i = 0;
    const char *s = linea;
    char **strv = calloc(strlen(linea) + 1, sizeof(char *));

    // Solamente números positivos,
    // Garantía de input siempre válido. Por tanto, es como un split en la barrera dígito/no-espacio.
    while (*s != '\0') {
        size_t n = 1;

        while (isspace(*s)) {
            s++;
        }

        if (isdigit(*s)) {
            n = strspn(s, "0123456789");
        }

        if (*s) {
            strv[i++] = strndup(s, n);
            s += n;
        }
    }

    return strv;
}