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

elemento crearElemStr(char*);
elemento crearElemInd(int);
elemento crearElemNull();
int crearTerceto1(elemento);
int crearTerceto3(elemento, elemento, elemento);
void imprimirTercetos();

/* Índice global para tercetos */
int indTercetos = 0;

/* Array de Tercetos */
terceto tercetos[900];

elemento crearElemStr(char* str) {
    elemento e;
    e.valor.cad = strdup(str);
    e.tipo = cadena;
    return e;
}

elemento crearElemInd(int ind) {
    elemento e;
    e.valor.ind = ind;
    e.tipo = indice;
    return e;
}

elemento crearElemNull() {
    elemento e;
    e.tipo = nulo;
    return e;
}

/*  Crear un terceto con los elementos pasados por parámetro y se Agregamos
    al array global de tercetos */
int crearTerceto3(elemento e1, elemento e2, elemento e3) {
    terceto t;
    int indice = indTercetos;

    t.elementos[0] = e1;
    t.elementos[1] = e2;
    t.elementos[2] = e3;

    /* Agregamos el terceto al array global de tercetos */
    tercetos[indice] = t;
    indTercetos++;

    /* Devolvemos el índice que apunta al terceto creado */
    return indice;
}

/*  Crear un terceto, donde el primer elemento es el valor pasado por parámetro.
    La diferencia con crearTerceto3, es que solo se asigna el primer elemento
    y el resto se asume que son valores nulos */
int crearTerceto1(elemento e) {
    terceto t;
    int indice = indTercetos;

    printf("%s", e.valor);
    t.elementos[0] = e;
    t.elementos[1] = crearElemNull();
    t.elementos[2] = crearElemNull();

    tercetos[indice] = t;
    indTercetos++;

    printf("\nEl indice es: %d", indice);
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
                fprintf(gci, "%s", e.valor.cad);
            } else if (e.tipo == indice) {
                fprintf(gci, "[%d]", e.valor.ind + 1);
            } else {
                fprintf(gci, "_");
            }

            if (j < 2) fprintf(gci, ", ");
        }
        fprintf(gci, ")\n");
    }
    fprintf(gci,"\n--- LISTA DE TERCETOS ---\n");
    
    if (fclose(gci) != 0) {
        printf("No se puede CERRAR el archivo de la lista de tercetos.");
        exit(1);
    }
}
