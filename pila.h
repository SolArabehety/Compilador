#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

/* Pila para almacenar enteros, necesario para CGI */
typedef struct nodoPila {
  int dato;
  struct nodoPila* sig;
} nodoPila;

typedef nodoPila* pila;

void inicializarPila(pila* p);
int pilaVacia(pila* p);
void apilar(pila* p, int val);
int mirarTope(pila* p);
int desapilar(pila* p);

void inicializarPila(pila* p) {
  *p = NULL;
}

int pilaVacia(pila* p) {
  return (*p) == NULL;
}

void apilar(pila* p, int val) {
  nodoPila* nodo = (nodoPila*)malloc(sizeof(nodoPila));

  nodo->dato = val;
  nodo->sig = ((*p) == NULL ? NULL : (*p));

  *p = nodo;
}

int desapilar(pila* p) {
  if(*p == NULL) {
    printf("Intento de desapilar una pila que estaba vacia.\n");
    return -1; 
  }

  int val = (*p)->dato;
  nodoPila* aux = *p;
  *p = (*p)->sig;
  free(aux);

  return val;
}

int mirarTope(pila* p) {
  return (*p)->dato;
}