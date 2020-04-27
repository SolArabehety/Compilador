#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <conio.h>
#include <stdio.h>


#define LIM_REAL 2147483647
#define LIM_INT 32768
#define LIM_STR 30

#define LIM_SIMBOLOS     100 // Cantidad de símbolos que va a poder manejar la tabla


typedef struct 
{   char nombre[100];
    char tipo  [20];
    char valor [100];
    int longitud;
} tablaDeSimbolos;

tablaDeSimbolos TOS[LIM_SIMBOLOS];
int TOStop = 0;    

void guardarTOS();
int buscarValorEnTS(char*);
int buscarNombreEnTS(char*);

void insertar_ID_en_Tabla(char*, int);
void insertar_ENTERO_en_Tabla(char*,int);
void insertar_REAL_en_Tabla(char*, double);
void insertar_STRING_en_Tabla(char*);

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
    fprintf(tos,"\n------------------------------ TABLA DE  SIMBOLOS ------------------------------\n");
    for (i=0; i< TOStop; i++)
    {
        sprintf(aux, "%d", TOS[i].longitud);
        if(strcmp(aux, "0") == 0)
            aux[0] = '\0';
        fprintf(tos,"Nombre: %s  | Tipo: %s   | Valor: %s   | Longitud: %s \n",TOS[i].nombre, TOS[i].tipo, TOS[i].valor, aux);
    }

    fprintf(tos,"\n------------------------------ TABLA DE  SIMBOLOS ------------------------------\n");
    
    if(fclose(tos)!=0)
    {
        printf("No se puede CERRAR el archivo de la tabla de simbolos");
        exit(1);
    }
}

int buscarValorEnTS(char* val)
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

void getNombreTOS(char* nombre, char*aux)
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

int buscarNombreEnTS(char* val)
{
    int i;
    for (i=0; i<TOStop; i++)
    {
        if(strcmp(TOS[i].nombre, val) == 0)
        {
            return 1;
        }
    }
    
    return 0;
}

// INSERTS ------------------------------------------------------------------------

// tipo = 1 - STRING
// tiop = 2 - INTEGER
// tipo = 3 - FLOAT
void insertar_ID_en_Tabla(char* token, int tipo)
{
	if(!buscarNombreEnTS(token))
	{
		strcpy(TOS[TOStop].nombre, token);
		
		switch(tipo){
		case 1:
			strcpy(TOS[TOStop].tipo, "STRING");
		
		break;
		
		case 2:
			strcpy(TOS[TOStop].tipo, "INTEGER");
			
		break;
		
		case 3:
			strcpy(TOS[TOStop].tipo, "FLOAT");
			
		break;
			
		}
	
		
		TOStop++;
	}
}



void insertar_ENTERO_en_Tabla(char* nombreToken,int token)
{			
	if(!buscarNombreEnTS(nombreToken))
	{
		char tokenString[100];
		sprintf(tokenString, "%d", token);		
		
		strcpy(TOS[TOStop].nombre,nombreToken);
		strcpy(TOS[TOStop].tipo,"CONST_INT");
	    strcpy(TOS[TOStop].valor, tokenString);

		TOStop++;
	}
}

void insertar_REAL_en_Tabla(char* nombreToken, double token)
{
	if(!buscarNombreEnTS(nombreToken))
	{
		char tokenString[100];
		sprintf(tokenString, "%lf", token);		
		
		strcpy(TOS[TOStop].nombre,nombreToken);
		strcpy(TOS[TOStop].tipo,"CONST_REAL");
	    strcpy(TOS[TOStop].valor, tokenString);

		TOStop++;
	}
}

void insertar_STRING_en_Tabla(char* token)
{
	if(!buscarValorEnTS(token))
	{
		strcpy(TOS[TOStop].nombre, "");
		strcpy(TOS[TOStop].tipo,"CONST_STRING" );
		strcpy(TOS[TOStop].valor, token);
		TOS[TOStop].longitud = (strlen(token));
	
		TOStop++;
	}
}


