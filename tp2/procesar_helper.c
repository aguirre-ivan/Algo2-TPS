#define _POSIX_C_SOURCE 200809L
#include "procesar_helper.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void free_strv(char* strv[]){
    size_t i = 0;
    while (strv[i] != NULL) free(strv[i++]);
    free(strv);
}

bool parse_num(const char *tok, calc_num *dest) {
    char *end;
    *dest = strtol(tok, &end, 10);
    return *end == '\0';
}

char **split(const char *str, char sep) {
	size_t largo = strlen(str);
	size_t contador = 0;
  	size_t ultima_pos = 0;

  	char **strv = malloc(sizeof(char*) * (largo + 2));

  	if (!strv) return NULL;

	size_t i;
  	for (i = 0; i < largo-1; i++) { // -1 para hacer rstrip '\n' al ultimo
  		if (str[i] == sep) {
  			strv[contador] = strndup(str + ultima_pos, i - ultima_pos);
  			ultima_pos = (i + 1);
  			contador++;
  		}
  	}

  	strv[contador] = strndup(str + ultima_pos, i - ultima_pos);
  	strv[contador + 1] = NULL;

  	return strv;
}