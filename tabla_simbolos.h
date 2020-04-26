#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <conio.h>
#include <stdio.h>

#define LIM_SIMBOLOS     100 // Cantidad de símbolos que va a poder manejar la tabla

typedef struct 
{   char nombre[100];
    char tipo  [11];
    char valor [100];
    int limite;
    int longitud;
} tablaDeSimbolos;

void guardarTOS();
void asignarTipo(int);
void insertar_ID_en_Tabla(char*);
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

void insertar_ID_en_Tabla(char* token)
{
    char aux[100];
    strcpy(aux,"_@");
    strcat(aux, token);
    if(!buscarEnTOSID(aux))
    {
        strcpy(TOS[TOStop].nombre, aux);
        strcpy(TOS[TOStop].tipo,"ID" );
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
