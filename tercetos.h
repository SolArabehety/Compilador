#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/*  Esta unión significa que elemento podra tener un char*
    o un int (para índices hacio otros tercetos), pero no ambos
    al mismo tiempo */
typedef union unionTerceto {
    char* cad;
    int ind;
} unionTerceto;

/*  Un terceto puede tener índices, strings o nada. */
typedef enum tipoValor {
    indice,
    cadena,
    nulo
} tipoValor;

/*  Esta struct es necesaria para poder determinar que tipo de 
    valor está siendo almacenado en la unión */
typedef struct elemento {
    tipoValor tipo;
    unionTerceto valor;
} elemento;

typedef struct terceto {
    elemento elementos[3];
} terceto;

elemento crearElementoStr(char*);
elemento crearElementoInd(int);
elemento crearElementoNull();
int crearTerceto(char*);
void imprimirTercetos();

/* Índice global para tercetos */
int indTercetos = 0;

/* Array de Tercetos */
terceto tercetos[900];

elemento crearElementoStr(char* str) {
    elemento e;
    e.valor.cad = strdup(str);
    e.tipo = cadena;
    return e;
}

elemento crearElementoInd(int ind) {
    elemento e;
    e.valor.ind = ind;
    e.tipo = indice;
    return e;
}

elemento crearElementoNull() {
    elemento e;
    e.tipo = nulo;
    return e;
}

int crearTerceto(char* str) {
    terceto t;
    int indice = indTercetos;

    t.elementos[0] = crearElementoStr(str);
    /*  Como solo se tiene 1 argumento, se asume que el resto 
        de los elementos del terceto son nulos */
    t.elementos[1] = crearElementoNull();
    t.elementos[2] = crearElementoNull();

    /* Agregamos el terceto al array global de tercetos */
    tercetos[indice] = t;
    indTercetos++;

    /* Devolvemos el índice que apunta al terceto creado */
    return indice;
}

void imprimirTercetos() {
    FILE *gci;
    int i, j;

    if ((gci = fopen("intermedia.txt", "w")) == NULL) {
        printf("No se puede crear el archivo de la lista de tercetos.");
        exit(1);
    }

    fprintf(gci, "\n--- LISTA DE TERCETOS ---\n\n");
    for (i = 0; i < indTercetos; i++) {
        fprintf(gci, "%d: (", i + 1);
        for (j = 0; j < 3; j++) {
            elemento e = tercetos[i].elementos[j];

            if (e.tipo == cadena) {
                fprintf(gci, "\"%s\"", e.valor.cad);
            } else if (e.tipo == indice) {
                fprintf(gci, "[%d]", e.valor.ind + 1);
            } else {
                fprintf(gci, "_");
            }

            fprintf(gci, ",");
        }
        fprintf(gci, ")\n");
    }
    fprintf(gci,"\n--- LISTA DE TERCETOS ---\n");
    
    if (fclose(gci) != 0) {
        printf("No se puede CERRAR el archivo de la lista de tercetos.");
        exit(1);
    }
}
