#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

extern int yylineno;

/*  Son los nombres de cada tipo */
const char* nombreTipos[7] = {"indefinido", "string", "entero", "real", "const string", "const entero", "const real"};

/*  Lo mismo que arriba, pero estos son los que se muestran en la tabla de símbolos. 
    El tipo UNDEFINED no debería aparecer nunca en la tabla igual. */
const char* nombreTiposTabla[7] = {"UNDEFINED", "STRING", "INTEGER", "FLOAT", "CONST_STRING", "CONST_INT", "CONST_FLOAT"};

/*  Para que sea mas facil de identificar los tipos de las variables que maneja 
    nuestro compilador. Los últimos 3 (los consts) no sun usados internamente, 
    solo son para diferenciarlos en la tabla de símbolos. 
    NOTA: El indefinido no es un tipo "válido", nada más es para asignarle a los
    tercetos donde no importa realmente de que tipo son. */
typedef enum tipoValor {
    indefinido,
    string,
    entero,
    real,
    constString,
    constEntero,
    constReal,
} tipoValor;

/*  Estas estructuras son para optimizar la generación de tercetos.
    Un índice puede apuntar a la lista de tercetos o a la lista
    de símbolos según sea su tipo. */
typedef enum tipoIndice {
    esSimbolo,
    esTerceto
} tipoIndice;

typedef struct indice {
    tipoIndice tipo;
    int num;
} indice;

void borrarChar(char*, char);

/* Pila para almacenar índices, necesario para CGI */
typedef struct nodoPila {
  indice dato;
  struct nodoPila* sig;
} nodoPila;

typedef nodoPila* pila;

void inicializarPila(pila* p);
int pilaVacia(pila* p);
void apilar(pila* p, indice val);
indice mirarTope(pila* p);
indice desapilar(pila* p);

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

void inicializarPila(pila* p) {
  *p = NULL;
}

int pilaVacia(pila* p) {
  return (*p) == NULL;
}

void apilar(pila* p, indice val) {
  nodoPila* nodo = (nodoPila*)malloc(sizeof(nodoPila));

  nodo->dato = val;
  nodo->sig = ((*p) == NULL ? NULL : (*p));

  *p = nodo;
}

indice desapilar(pila* p) {
  if(*p == NULL) {
    printf("ERROR: Intento de desapilar una pila que estaba vacia.\n");
    exit(1);
  }

  indice val = (*p)->dato;
  nodoPila* aux = *p;
  *p = (*p)->sig;
  free(aux);

  return val;
}

indice mirarTope(pila* p) {
  return (*p)->dato;
}
