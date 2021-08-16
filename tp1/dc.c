#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR "ERROR\n"

bool operacion(pilanum_t* pila_numeros, calc_token* token, calc_num* resultado) {
    int arity = token->oper.arity;

    if (pilanum_esta_vacia(pila_numeros)) return false;
    
    calc_num* num1 = malloc(sizeof(calc_num));
    if (num1 == NULL) return false;
    if (!desapilar_num(pila_numeros, num1)) {
        free(num1);
        return false;
    }
   
    if (arity == 1) {
        if ((*num1) < 0) {
            free(num1);
            return false;
        }
        *resultado = sqrt(*num1);
        free(num1);
        return true;
    }

    calc_num* num2 = malloc(sizeof(calc_num));
    if (num2 == NULL) {
        free(num1);
        return false;
    }
    if (!desapilar_num(pila_numeros, num2)) {
        free(num1); free(num2);
        return false;
    } 

    if (arity == 2) {
        if (token->oper.op == OP_ADD) *resultado = *num1 + *num2;
        if (token->oper.op == OP_SUB) *resultado = *num2 - *num1;
        if (token->oper.op == OP_MUL) *resultado = (*num1) * (*num2);
        if (token->oper.op == OP_POW) {
            if (*num1 < 0) {
                free(num1); free(num2);
                return false;
            }
            *resultado = pow(*num2, *num1);
        }
        if (token->oper.op == OP_LOG) {
            if (*num1 < 2) {
                free(num1); free(num2);
                return false;
            }
            *resultado = (log(*num2)) / (log(*num1));
        }
        if (token->oper.op == OP_DIV) {
            if (*num1 == 0) {
                free(num1); free(num2);
                return false;
            }
            *resultado = *num2 / *num1;
        }
        free(num1); free(num2);
        return true;
    }

    calc_num* num3 = malloc(sizeof(calc_num));
    if (num3 == NULL) {
        free(num1); free(num2);
        return false;
    }
    if (!desapilar_num(pila_numeros, num3)) {
        free(num1); free(num2); free(num3);
        return false;
    }

    if (arity == 3) {
        //El unico con arity 3 es el ternario
        *resultado = *num3 ? *num2 : *num1;
        free(num1); free(num2); free(num3);
        return true;
    }

    return false;
}

bool calculo_de_linea(char** strv_linea, calc_num* resultado) {
    pilanum_t* pila_numeros = pilanum_crear();
    if (pila_numeros == NULL) return false;

    size_t i = 0;
    while (strv_linea[i] != NULL) {

        calc_token* token = malloc(sizeof(calc_token));
        if (!calc_parse(strv_linea[i], token)) {
            pilanum_destruir(pila_numeros);
            free(token);
            return false;
        }

        if (token->type == TOK_NUM) {
            apilar_num(pila_numeros, token->value);
        } else {
            calc_num* resultado_parcial = malloc(sizeof(calc_num));
            if (!operacion(pila_numeros, token, resultado_parcial)) {
                pilanum_destruir(pila_numeros);
                free(token);
                free(resultado_parcial);
                return false;
            }
            apilar_num(pila_numeros, *resultado_parcial);
            free(resultado_parcial);
        }

        i++;
        free(token);
    }

    bool ok = (desapilar_num(pila_numeros, resultado) && pilanum_esta_vacia(pila_numeros));
    pilanum_destruir(pila_numeros);
    return ok;
}

int main(void) {
    char* linea = NULL;
    size_t tam = 0;
    
    while (getline(&linea, &tam, stdin) != EOF) {
        char** linea_split = dc_split(linea);
        calc_num* resultado = calloc(1, sizeof(calc_num));
        if (linea_split[0] == NULL) {
            fprintf(stdout, ERROR);
        } else {
            if (calculo_de_linea(linea_split, resultado)) {
                fprintf(stdout, "%ld\n", *resultado);
            } else {
                fprintf(stdout, ERROR);
            }
        }
        free_strv(linea_split);
        free(resultado);
    }
    free(linea);
    return 0;
}