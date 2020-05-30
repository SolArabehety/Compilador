#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <conio.h>
#include <stdio.h>
#include "util.h"

#define LIM_REAL 2147483647
#define LIM_INT 32768
#define LIM_STR 30

#define LIM_SIMBOLOS     100 // Cantidad de símbolos que va a poder manejar la tabla


typedef struct 
{   char nombre[100];
    tipoValor tipo;
    char valor [100];
    int longitud;
} tablaDeSimbolos;

tablaDeSimbolos TOS[LIM_SIMBOLOS];
int TOStop = 0;    
char nombreToken[100] = "";

void guardarTOS();
int buscarValorEnTS(const char*);
int buscarNombreEnTS(const char*);

int insertar_ID_en_Tabla(const char*, tipoValor);
int insertar_ENTERO_en_Tabla(int);
int insertar_REAL_en_Tabla(double);
int insertar_STRING_en_Tabla(const char*);
const char* obtenerNombreSimbolo(int);
tipoValor obtenerTipoSimbolo(int);
indice buscarIndiceSimbolo(const char*);
// Funciones generales de la TS ------------------------------------------------------------------------

void guardarTOS()
{
    int i;
    char aux[100];
    FILE *tos;         
    if ((tos = fopen ("TablaDeSimbolos.txt","w"))== NULL)
    {
        printf("No se puede crear el archivo de la tabla de simbolos");
        exit(1);
    }
    fprintf(tos, "\n--------------------------------------------- TABLA DE  SIMBOLOS ---------------------------------------------\n\n");
    fprintf(tos, "Nombre: %-25s    ", "");
    fprintf(tos, "Tipo: %-15s    ", "");
    fprintf(tos, "Valor: %-25s    ", "");
    fprintf(tos, "Longitud: %-10s ", "");
    fprintf(tos, "\n______________________________________________________________________________________________________________\n");
    for (i=0; i< TOStop; i++)
    {
        sprintf(aux, "%d", TOS[i].longitud);
        if(strcmp(aux, "0") == 0)
            aux[0] = '\0';
        fprintf(tos,"%-33s  | %-21s  | %-32s  | %-20s \n", TOS[i].nombre, nombreTiposTabla[TOS[i].tipo], TOS[i].valor, aux);
    }

    fprintf(tos,"\n--------------------------------------------- TABLA DE  SIMBOLOS ---------------------------------------------\n");
    
    if(fclose(tos)!=0)
    {
        printf("No se puede CERRAR el archivo de la tabla de simbolos");
        exit(1);
    }
}

int buscarValorEnTS(const char* val)
{
	int i;
	for (i=0; i<TOStop; i++)
    {
		if(strcmp(TOS[i].valor, val) == 0)
		{
			return 1;
		}
	}
	
	return 0;
}

void getNombreTOS(const char* nombre, char*aux)
{
	int i;
	for (i=0; i<TOStop; i++)
    {
		if(strcmp(TOS[i].valor, nombre) == 0)
		{
			strcpy(aux,TOS[i].nombre);
			return;
		}
	}
	
	strcpy(aux,"");
	return;
}

tablaDeSimbolos* getDirTOSPorIndice(int indice) {
    if (indice > TOStop) {
        printf("Error: Se intentó acceder a un indice en la tabla de simbolos que no existe: %d", indice);
        exit(2);
    }
	return &TOS[indice];
}

int buscarNombreEnTS(const char* val)
{
    int i;
    for (i=0; i<TOStop; i++)
    {
        if(strcmp(TOS[i].nombre, val) == 0)
        {
            return i;
        }
    }
    /* -1 quiere decir el el símbolo no existe en la tabla */
    return -1;
}

// INSERTS ------------------------------------------------------------------------
/*  Los tipos están definidos en la struct tipoVar en util.h, pero básicamente son:
    tipo = 0 -> INDEFINIDO
    tipo = 1 -> STRING
    tiop = 2 -> ENTERO
    tipo = 3 -> REAL    */
int insertar_ID_en_Tabla(const char* token, tipoValor tipo)
{
    int TOSaux = TOStop;
    int indiceTS;
    // printf("\nAgregando ID en table: %s, %d\n", token, tipo);
    if((indiceTS = buscarNombreEnTS(token)) == -1) {
        strcpy(TOS[TOStop].nombre, token);
        TOS[TOStop].tipo = tipo;
        TOStop++;
    } else {
        return indiceTS;
    }
    return TOSaux;
}

int insertar_ENTERO_en_Tabla(int valor)
{		
    int TOSaux = TOStop;
    int indiceTS;
    char valorString[100];
    char nombreSimbolo[100];
    sprintf(valorString, "%d", valor);

    if(strcmp(nombreToken, "") == 0) {
        // si no hay nombre token, entonces es una constante sin nombre
        // le inventamos nombre con un "_" y el valor
        strcpy(nombreSimbolo, "_");
        strcat(nombreSimbolo, valorString);
    } else {
        // si hay nombre token, entonces es una constante con nombre
        // usamos el nombreToken como nombre del símbolos
        strcpy(nombreSimbolo, nombreToken);
        // 'destruimos' el nombreToken para que no sea reusado por accidente
        strcpy(nombreToken, "");
    }

    // printf("\nAgregando ENTERO en table: %s, %d\n", nombreSimbolo, valor);
    if((indiceTS = buscarNombreEnTS(nombreSimbolo)) == -1) {
        strcpy(TOS[TOStop].nombre, nombreSimbolo);
        strcpy(TOS[TOStop].valor, valorString);
        TOS[TOStop].tipo = constEntero;

        TOStop++;
    } else {
        return indiceTS;
    }
    return TOSaux;
}

int insertar_REAL_en_Tabla(double valor)
{
    int TOSaux = TOStop;
    int indiceTS;
    char valorString[100];
    char nombreSimbolo[100];
    sprintf(valorString, "%lf", valor);

    if(strcmp(nombreToken, "") == 0) {
        strcpy(nombreSimbolo, "_");
        strcat(nombreSimbolo, valorString);
    } else {
        strcpy(nombreSimbolo, nombreToken);
        strcpy(nombreToken, "");
    }

    // printf("\nAgregando REAL en table: %s, %f\n", nombreSimbolo, valor);
    if((indiceTS = buscarNombreEnTS(nombreSimbolo)) == -1) {
        strcpy(TOS[TOStop].nombre, nombreSimbolo);
        strcpy(TOS[TOStop].valor, valorString);
        TOS[TOStop].tipo = constReal;

        TOStop++;
    } else {
        return indiceTS;
    }
    return TOSaux;
}

int insertar_STRING_en_Tabla(const char* str)
{
    int TOSaux = TOStop;
    int indiceTS;
    char nombreSimbolo[100];
    char* valor = strdup(str);

    // Para borrar las "" que quedan cuando se inserta una constante string
    borrarChar(valor, '"');

    if(strcmp(nombreToken, "") == 0) {
        strcpy(nombreSimbolo, "_");
        strcat(nombreSimbolo, valor);
    } else {
        strcpy(nombreSimbolo, nombreToken);
        strcpy(nombreToken, "");
    }

    // printf("\nAgregando STRING en table: %s, %s\n", nombreSimbolo, valor);
    if((indiceTS = buscarNombreEnTS(nombreSimbolo)) == -1) {
        strcpy(TOS[TOStop].nombre, nombreSimbolo);
        strcpy(TOS[TOStop].valor, valor);
        TOS[TOStop].longitud = (strlen(valor));
        TOS[TOStop].tipo = constString;

        TOStop++;
    } else {
        return indiceTS;
    }
    return TOSaux;
}

/*  Función para obtener el nombre de un símbolo buscándolo en la tabla
    según su índice. */
const char* obtenerNombreSimbolo(int ind) {
    return TOS[ind].nombre;
} 

/*  Función para obtener el tipo de un símbolo buscándolo en la tabla
    según su índice. */
tipoValor obtenerTipoSimbolo(int ind) {
    tipoValor tipo = TOS[ind].tipo;

    /*  Si son constantes, simplemente devolvemos sus tipos normales.
        Esto es porque en la parte de CGI no importa si son constantes
        o no. */
    if (tipo == constString) {
        tipo = string;
    } else if (tipo == constEntero) {
        tipo = entero;
    } else if (tipo == constReal) {
        tipo = real;
    }

    return tipo;
}

/*  Función para obtener un indice que apunte a un símbolo determinado. */
indice buscarIndiceSimbolo(const char* nombre) {
    indice ind;
    ind.num = buscarNombreEnTS(nombre);
    ind.tipo = esSimbolo;

    return ind;
}
