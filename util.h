#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

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
