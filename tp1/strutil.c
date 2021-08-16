#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* substr(const char *str, size_t n){
    char* res = strndup(str, n);
    return res;
}

char** split(const char* str, char sep){
    size_t tam, n_sep = 0;
    for (int i=0; str[i] != '\0'; i++) {
        if (str[i] == sep) n_sep++;
    }
    tam = n_sep + 1; //El tam no incluye el NULL

    char** res = malloc(sizeof(char*) * (tam+1));
    if (res == NULL) return NULL;
    res[tam] = NULL;

    size_t i_palabra = 0, i_char_actual = 0, i_char_anterior = 0;
    while (i_palabra < tam) {
        while (str[i_char_actual] != sep && str[i_char_actual] != '\0') {
            i_char_actual++;
        }
        char* res_parcial = strndup(str+i_char_anterior, i_char_actual-i_char_anterior);
        res[i_palabra] = res_parcial;
        if (res[i_palabra] == NULL) {
            fprintf(stderr, "Ocurrio un error en el copiado en resultado[%ld]\n", i_palabra);
            free_strv(res);
        }
        i_char_anterior = ++i_char_actual; //Saltea el separador
        i_palabra++;
    }
    return res;
}

char* join(char** strv, char sep){
    size_t len = 0, n_char = 0, n_sep = 0;
    while (strv[len] != NULL) {
        n_char += strlen(strv[len]);
        len++; //El len final no incluye el NULL
    }
    n_sep = len+1;

    char* res = calloc((n_char+n_sep+1),sizeof(char));
    if (res == NULL) return NULL;
    char* cadena_sep = malloc(sizeof(char)*2);
    if (cadena_sep == NULL) {
        free(res);
        return NULL;
    }
    cadena_sep[0] = sep;
    cadena_sep[1] = '\0';

    for (int i = 0; i<len; i++) {
        char* puntero_a_final = i == 0 ? stpcpy(res, strv[i]) : stpcpy(puntero_a_final, strv[i]);
        if (puntero_a_final == NULL) {
            fprintf(stderr, "Hubo un error al intentar hacer join, en el elemento [%d] del strv", i);
            free(res);
            free(cadena_sep);
            return NULL;
        }
        if (i != len-1) {
            puntero_a_final = stpcpy(puntero_a_final, cadena_sep);
            if (puntero_a_final == NULL) {
            fprintf(stderr, "Hubo un error al intentar hacer join, al querer appendear &sep");
            free(res);
            free(cadena_sep);
            return NULL;
            }
        }
    }
    free(cadena_sep);
    return res;
}

void free_strv(char* strv[]){
    size_t i = 0;
    while (strv[i] != NULL) free(strv[i++]);
    free(strv);
}
