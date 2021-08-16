#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR_EN_COLA "ERROR operando con la colatok\n"

void imprimir_cola(FILE* f_salida, colatok_t* cola) {
    while (!colatok_esta_vacia(cola)) {
        calc_token* token_desencolado = desencolar_token(cola);

        if (token_desencolado->type == TOK_NUM) {
            fprintf(f_salida, "%ld ", token_desencolado->value);
        } else { 
            fprintf(f_salida, "%c ", token_desencolado->oper.char_op);
        }

        free(token_desencolado);
    }
    fprintf(f_salida, "\n");
}

void calculo_de_linea(char** strv_linea, colatok_t* cola_resultado) {
    size_t i = 0;
    pilatok_t* pila_op = pilatok_crear();

    while(strv_linea[i] != NULL) {
        
        calc_token* token = calloc(1, sizeof(calc_token));

        calc_parse(strv_linea[i], token);

        if (token->type == TOK_NUM) {
            encolar_token(cola_resultado, token);
        }
        
        else if (token->type == TOK_LPAREN) {
            apilar_token(pila_op, token);
        }

        else if (token->type == TOK_RPAREN) {
            while (pilatok_ver_tope(pila_op)->type != TOK_LPAREN) {
                calc_token* token_aux = desapilar_token(pila_op);
                encolar_token(cola_resultado, token_aux);
            }
            //Desapilo el parentesis abierto y lo borro
            calc_token* token_borrar = desapilar_token(pila_op);
            free(token_borrar); //LPAREN
            free(token); //RPAREN
        }

        else if (pilatok_esta_vacia(pila_op)) {
            apilar_token(pila_op, token);
        }

        else {
            while (!pila_esta_vacia(pila_op)) {

                unsigned preced_token = token->oper.precedence;
                unsigned preced_tope = pilatok_ver_tope(pila_op)->oper.precedence;
                int asoc_token = token->oper.associativity;
                
                if ((asoc_token == ASSOC_RIGHT && preced_token <= preced_tope) || (asoc_token == ASSOC_LEFT && preced_token < preced_tope)) {
                    calc_token* token_aux = desapilar_token(pila_op);
                    encolar_token(cola_resultado, token_aux);
                } else {
                    break;
                }
            }

            apilar_token(pila_op, token);
        }

        i++;
    }
    
    while (!pilatok_esta_vacia(pila_op)) {
        calc_token* token_aux = desapilar_token(pila_op);
        encolar_token(cola_resultado, token_aux);
    }

    pilatok_destruir(pila_op);
}

int main(void) {
    char* linea = NULL;
    size_t tam = 0;
    
    while (getline(&linea, &tam, stdin) != EOF) {

        char** linea_split = infix_split(linea);

        colatok_t* cola_resultado = colatok_crear();
        if (cola_resultado == NULL) {
            free(linea_split); free(cola_resultado);
            fprintf(stderr, ERROR_EN_COLA);
            return -1;
        }
        
        calculo_de_linea(linea_split, cola_resultado);
        imprimir_cola(stdout, cola_resultado);
        colatok_destruir(cola_resultado);
        free_strv(linea_split);
    }

    free(linea);
    return 0;
}