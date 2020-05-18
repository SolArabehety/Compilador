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
elemento crearElemInt(int);
elemento crearElemNull();
int crearTerceto(elemento, elemento, elemento);
int crearTercetoConstante(char*);
int crearTercetoOperacion(char*, int, int);
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

elemento crearElemInt(int ind) {
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

/*  Busca un terceto por el nombre de una constante en el primer elemento y
    devuelve su índice, de no encontrarlo (no existe) devuelve -1. */
int buscarTerceto(const char* val) {
    int i;

    for (i = 0; i < indTercetos; i++) {
        if (strcmp(val, tercetos[i].elementos[0].valor.cad) == 0)
            return i;
    }

    /* Si no se encontró el terceto, se devuelve -1 */
    return -1;
}

/*  Crear un terceto con los elementos pasados por parámetro y se Agregamos
    al array global de tercetos. Esta es una función genérica para insertar
    un terceto. Las demás funciones son más específicas y llaman a esta. */
int crearTerceto(elemento e1, elemento e2, elemento e3) {
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
    La diferencia con crearTerceto, es que solo se asigna el primer elemento
    y el resto se asume que son valores nulos.
    Se utiliza para las constantes. */
int crearTercetoConstante(char* val) {
    /* Antes de crearlo, nos fijamos si ya existe */
    int idx = buscarTerceto(val);
    if (idx != -1)
        return idx;

    return crearTerceto(crearElemStr(val), crearElemNull(), crearElemNull());
}

/*  Crear un terceto, donde el primer elemento es una string con el valor
    de alguna operación (ejemplo, "+") y los otros 2 son valores int que
    corresponden a los índices de los tercetos sobre los que se realiza
    la operación. */
int crearTercetoOperacion(char* op, int ind1, int ind2) {
    return crearTerceto(crearElemStr(op), crearElemInt(ind1), crearElemInt(ind2));
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
