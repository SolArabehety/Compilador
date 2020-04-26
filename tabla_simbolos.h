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
    char tipo  [11];
    char valor [100];
    int limite;
    int longitud;
} tablaDeSimbolos;

void guardarTOS();
void insertar_ID_en_Tabla(char*, int);
int buscarEnTOSID(char*);

int TOStop = 0;    
tablaDeSimbolos TOS[LIM_SIMBOLOS];

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
        fprintf(tos,"Nombre: %s  | Tipo: %s   | Valor: %s | Limite: %d | Longitud: %s \n",TOS[i].nombre, TOS[i].tipo, TOS[i].valor, TOS[i].limite, aux);
    }

    fprintf(tos,"\n------------------------------ TABLA DE  SIMBOLOS ------------------------------\n");
    
    if(fclose(tos)!=0)
    {
        printf("No se puede CERRAR el archivo de la tabla de simbolos");
        exit(1);
    }
}


// tipo = 1 - STRING
// tiop = 2 - INTEGER
// tipo = 3 - FLOAT
void insertar_ID_en_Tabla(char* token, int tipo)
{
	if(!buscarEnTOSID(token))
	{
		strcpy(TOS[TOStop].nombre, token);
		
		switch(tipo){
		case 1:
			strcpy(TOS[TOStop].tipo, "STRING");
			TOS[TOStop].limite = LIM_STR;
		break;
		
		case 2:
			strcpy(TOS[TOStop].tipo, "INTEGER");
			TOS[TOStop].limite = LIM_INT;
		break;
		
		case 3:
			strcpy(TOS[TOStop].tipo, "FLOAT");
				TOS[TOStop].limite = LIM_REAL;
		break;
			
		}
	
		
		TOStop++;
	}
}

int buscarEnTOSID(char* val)
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
