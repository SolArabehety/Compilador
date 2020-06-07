#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef enum tipoComparacion {
    comparacionSimple,
    comparacionDobleOR,
    comparacionDobleAND,
} tipoComparacion;

/* Pila para almacenar enteros, necesario para CGI */
typedef struct nodoPilaInt {
  int dato;
  struct nodoPilaInt* sig;
} nodoPilaInt;

typedef nodoPilaInt* pilaInt;

void inicializarPilaInt(pilaInt* p);
int pilaIntVacia(pilaInt* p);
void apilarInt(pilaInt* p, int val);
int mirarTopeInt(pilaInt* p);
int desapilarInt(pilaInt* p);

void inicializarPilaInt(pilaInt* p) {
  *p = NULL;
}

int pilaIntVacia(pilaInt* p) {
  return (*p) == NULL;
}

void apilarInt(pilaInt* p, int val) {
  nodoPilaInt* nodo = (nodoPilaInt*)malloc(sizeof(nodoPilaInt));

  nodo->dato = val;
  nodo->sig = ((*p) == NULL ? NULL : (*p));

  *p = nodo;
}

int desapilarInt(pilaInt* p) {
  if(*p == NULL) {
    printf("Intento de desapilar una pila que estaba vacia.\n");
    return -1; 
  }

  int val = (*p)->dato;
  nodoPilaInt* aux = *p;
  *p = (*p)->sig;
  free(aux);

  return val;
}

int mirarTopeInt(pilaInt* p) {
  return (*p)->dato;
}
