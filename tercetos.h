#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "tabla_simbolos.h"

/*  Esta unión significa que elemento podra tener un char*
    o un int (para índices hacio otros tercetos), pero no ambos
    al mismo tiempo */
typedef union unionTerceto {
    char* cad;
    int ind;
} unionTerceto;

/*  Esta struct es necesaria para poder determinar que tipo de 
    valor está siendo almacenado en la unión */
typedef struct elemento {
    /* Los tercetos solo deberían poder tener elementos que sean de tipo
    string, entero (para índices) o nada (indefinido) */
    tipoValor tipo;
    unionTerceto valor;
} elemento;

typedef struct terceto {
    elemento elementos[3];

    /*  Si es real, entero o cadena.
        Esto es un rebusque que hice para que fuera más fácil verificar el tipo
        en expresiones y asignaciones. No aparece en el archivo intermedia.txt y
        no es necesario que todos los tercetos lo tengan definido. */
    tipoValor tipoTerceto;
} terceto;

elemento crearElemStr(const char*);
elemento crearElemInt(int);
elemento crearElemNull();
int crearTerceto(elemento, elemento, elemento, tipoValor);
int crearTercetoConstanteEntera(int);
int crearTercetoConstanteString(const char*);
int crearTercetoConstanteReal(float);
int crearTercetoVariable(const char*, tipoValor);
int crearTercetoAsignacion(int, int);
int crearTercetoOperacion(const char*, int, int);
int crearTercetoBranch(const char*, int);
void modificarSaltoTerceto(int, int);
void imprimirTercetos();

/* Índice global para tercetos */
int indTercetos = 0;

/* Array de Tercetos */
terceto tercetos[900];

elemento crearElemStr(const char* str) {
    elemento e;
    e.valor.cad = strdup(str);
    e.tipo = string;
    return e;
}

elemento crearElemInt(int ind) {
    elemento e;
    e.valor.ind = ind;
    e.tipo = entero;
    return e;
}

elemento crearElemNull() {
    elemento e;
    e.tipo = indefinido;
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
int crearTerceto(elemento e1, elemento e2, elemento e3, tipoValor tipo) {
    terceto t;
    int indice = indTercetos;

    t.elementos[0] = e1;
    t.elementos[1] = e2;
    t.elementos[2] = e3;
    t.tipoTerceto = tipo;

    /* Agregamos el terceto al array global de tercetos */
    tercetos[indice] = t;
    indTercetos++;

    /* Devolvemos el índice que apunta al terceto creado */
    return indice;
}

/*  Crear un terceto, donde el primer elemento será el valor pasado por parámetro.
    La diferencia con crearTerceto, es que solo se asigna el primer elemento
    y el resto se asume que son valores nulos.
    Se utiliza para las constantes. */
int crearTercetoConstanteEntera(int val) {
    /* Antes de crearlo, nos fijamos si ya existe */
    char buffer[900];
    sprintf(buffer, "_%d", val);
    int idx = buscarTerceto(buffer);
    if (idx != -1)
        return idx;

    insertar_ENTERO_en_Tabla(val);
    return crearTerceto(crearElemStr(buffer), crearElemNull(), crearElemNull(), entero);
}

int crearTercetoConstanteString(const char* val) {
    char buffer[900];
    sprintf(buffer, "_%s", val);
    borrarChar(buffer, '"');
    int idx = buscarTerceto(buffer);
    if (idx != -1)
        return idx;

    insertar_STRING_en_Tabla(val);
    return crearTerceto(crearElemStr(buffer), crearElemNull(), crearElemNull(), string);
}

int crearTercetoConstanteReal(float val) {
    char buffer[900];
    sprintf(buffer, "_%f", val);
    int idx = buscarTerceto(buffer);
    if (idx != -1)
        return idx;

    insertar_REAL_en_Tabla(val);
    return crearTerceto(crearElemStr(buffer), crearElemNull(), crearElemNull(), real);
}

/*  Crear un terceto, donde el primer elemento es el valor pasado por parámetro y
    el segundo parámetro indicará que tipo tendrá en la tabla de símbolos.
    Es como crearTercetoConstante, pero la diferencia es que ademas se agrega
    a la tabla de símbolos. Se usa para las variables auxiliares. */
int crearTercetoVariable(const char* val, tipoValor tipo) {
    /* Antes de crearlo, nos fijamos si ya existe */
    int idx = buscarTerceto(val);
    if (idx != -1)
        return idx;
    
    insertar_ID_en_Tabla(val, tipo);

    return crearTerceto(crearElemStr(val), crearElemNull(), crearElemNull(), tipo);
}

/*  Crear un terceto, donde el primer y segundo elementos son indices de 
    tercetos. Crea una operación de asignación ("=", ind1, ind2) pero antes
    valida que la asignación pueda realizarse */
int crearTercetoAsignacion(int ind1, int ind2) {
    tipoValor tipo1 = tercetos[ind1].tipoTerceto;
    tipoValor tipo2 = tercetos[ind2].tipoTerceto;

    if (tipo1 == tipo2 || (tipo1 == real && tipo2 == entero)) {
        return crearTerceto(crearElemStr("="), crearElemInt(ind1), crearElemInt(ind2), tipo1);
    } else {
        printf("\nError en la linea %d: Se intento asignar un %s a una variable de tipo %s.", 
                yylineno, strTipos[tipo2], strTipos[tipo1]);
        exit(1);
        return 0;
    }
}

/*  Crear un terceto, donde el primer elemento es una string con el valor
    de alguna operación (ejemplo, "+") y los otros 2 son valores int que
    corresponden a los índices de los tercetos sobre los que se realiza
    la operación. */
int crearTercetoOperacion(const char* op, int ind1, int ind2) {
    tipoValor tipo1 = tercetos[ind1].tipoTerceto;
    tipoValor tipo2 = tercetos[ind2].tipoTerceto;
    tipoValor tipoResultado;

    if (tipo1 == tipo2) {
        tipoResultado = tipo1;
    } else if ((tipo1 == real && tipo2 == entero) || (tipo1 == entero && tipo2 == real)) {
        tipoResultado = real;
    } else {
        printf("\nError Error en la linea %d: La operacion %s entre %s y %s no es compatible.", 
                yylineno, op, strTipos[tipo1], strTipos[tipo2]);
        exit(1);
        return 0;
    }
    
    return crearTerceto(crearElemStr(op), crearElemInt(ind1), crearElemInt(ind2), tipoResultado);
}

/*  Crear un terceto, donde el primer elemento es el primre valor pasado por 
    parámetro, este debería ser una instrucción de assembler para hacer un 
    salto. El segundo parámetro es el índice del terceto hacia el cuál va a
    hacer el salto. En algunos casos no se conoce el salto al momento de crear
    el branch, en ese caso se debe colocar 0 en el salto y luego se deberá 
    usar la función modificarSaltoTerceto */
int crearTercetoBranch(const char* op, int salto) {
    return crearTerceto(crearElemStr(op), crearElemInt(salto), crearElemNull(), indefinido);
}

/*  Busca al terceto por índice según el primer parámetro de la función,
    y le cambia su segundo valor al entero pasado como segundo parámetro.
    Se debería utilizar para modificar los tercetos que tienen instrucciones
    de Branch */
void modificarSaltoTerceto(int ind, int salto) {
    tercetos[ind].elementos[1].valor.ind = salto;
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

            switch (e.tipo) {
                case string:
                    fprintf(gci, "%s", e.valor.cad);
                    break;
                case entero: /* Si es entero, es un índice y le sumamos 1 al mostrarlo */
                    fprintf(gci, "[%d]", e.valor.ind + 1);
                    break;
                default:
                    fprintf(gci, "_");
            }

            if (j < 2) {
                fprintf(gci, ", ");
            } 
        }
        fprintf(gci, ")\n");
    }
    fprintf(gci,"\n--- LISTA DE TERCETOS ---\n");
    
    if (fclose(gci) != 0) {
        printf("No se puede CERRAR el archivo de la lista de tercetos.");
        exit(1);
    }
}
