#include "strutil.c"

void pruebas_substr(){
    char* s1 = substr("Hola mundo", 6);
    char* s2 = substr("Algoritmos", 30);
    char* s3 = substr("", 2);
    char* ejemplo = "Ejemplo";
    char* s4 = substr(ejemplo, 2);
    char* s5 = substr(ejemplo + 4, 2);
    printf("La cadena es:");
    fprintf(stdout, "%s", s1);
    printf("|FIN DE CADENA\n\n");
    printf("La cadena es:");
    fprintf(stdout, "%s", s2);
    printf("|FIN DE CADENA\n\n");
    printf("La cadena es:");
    fprintf(stdout, "%s", s3);
    printf("|FIN DE CADENA\n\n");
    printf("La cadena es:");
    fprintf(stdout, "%s", s4);
    printf("|FIN DE CADENA\n\n");
    printf("La cadena es:");
    fprintf(stdout, "%s", s5);
    printf("|FIN DE CADENA\n\n");
    free(s1);
    free(s2);
    free(s3);
    free(s4);
    free(s5);
}

void pruebas_join(){
    char **s1 = split("abc,def", ',');
    char *res_s1 = join(s1, '\0');
    printf("Resultado 1:|%s|\n", res_s1);
    char **s2 = split("Hola mundo", ' ');
    char *res_s2 = join(s2, ',');
    printf("Resultado 2:|%s|\n", res_s2);
    free_strv(s1);
    free_strv(s2);
    free(res_s1);
    free(res_s2);
}

void pruebas_split(){
    char** s1 = split(",", ',');
    size_t i = 0;
    while (s1[i] != NULL) {
        printf("CADENA|%s|\n", s1[i]);
        i++;
    }
    if (s1[i] == NULL) printf("Aca va el NULL; %ld\n", i);
    free_strv(s1);
}

// descomentar prueba

int main() {
    //pruebas_substr();
    //pruebas_split();
    pruebas_join();
    return 0;
}
