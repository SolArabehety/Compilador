#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

/*  Para que sea mas facil de identificar los tipos de
    las variables que maneja nuestro compilador */
typedef enum tipoValor {
    indefinido,
    string,
    entero,
    real
} tipoValor;

void borrarChar(char*, char);

void borrarChar(char* s, char c) { 
    int n = strlen(s); 
    int i, j;

    for (i = j = 0; i < n; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        } 
    }

    s[j] = '\0'; 
}
