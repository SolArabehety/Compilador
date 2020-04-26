
%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "y.tab.h"


int yystopparser=0;
FILE  *yyin;


#define LIM_REAL 2147483647
#define LIM_INT 32768
#define LIM_STR 30

typedef struct 
{   char nombre[100];
    char tipo  [11];
    char valor [100];
    int limite;
    int longitud;
} tablaDeSimbolos;


// char tokens[100][100];  
// int indexTokens = 0; 	
// void guardarTokens(char*);


int TOStop = 0;	
tablaDeSimbolos TOS[100];


void guardarTOS();
void asignarTipo(int);
void insertar_ID_en_Tabla(char*);
int buscarEnTOSID(char*);
%}

%union {
	int int_val;
	double float_val;
	char *str_val;
}

%token ID COMENTARIO_INICIO COMENTARIO_FIN COMENTARIOS CADENA REAL ENTERO 
%token OP_SUMA OP_RESTA OP_MUL OP_DIV ASIG P_A P_C LL_A LL_C P_Y_C COMA OP_MAY_IG OP_MEN_IG 
%token OP_MAY OP_MEN OP_DISTINTO C_A C_C DOSPUNTOS OP_IGUAL ASIG_MAS ASIG_MEN ASIG_MULT ASIG_DIV
%token DEFVAR ENDDEF WRITE IF THEN ELSE ENDIF WHILE ENDWHILE FLOAT INTEGER AND OR NOT STRING READ COMB FACT
%%

programa:  	   
	{ printf("Inicia COMPILADOR\n");} 
	est_declaracion algoritmo    
	{	guardarTOS();
		printf("Fin COMPILADOR ok\n");
	};

est_declaracion:
	DEFVAR{printf("		DECLARACIONES\n");} declaraciones ENDDEF {printf("    Fin de las Declaraciones\n");}
	;

		
declaraciones:   
			declaracion
			| declaraciones declaracion
			;
	

declaracion:
			tipo_variable DOSPUNTOS lista_declaracion  {printf("	DECLARACION\n");}
			;

tipo_variable:
		STRING		{			} 
	| 	INTEGER 	{					};
	|	FLOAT		{				} 

lista_declaracion:  
				ID P_Y_C lista_declaracion {  insertar_ID_en_Tabla($<str_val>$);}
				|ID  {  insertar_ID_en_Tabla($<str_val>$);}
				;

 
algoritmo: 
         {printf("      COMIENZO de BLOQUES\n");} bloque
         ;

bloque:  
      sentencia
      |bloque sentencia
      ;

sentencia:
	 ciclo
	 |seleccion  
	 |asignacion
	 |salida_pantalla
         |ingreso_valor      
	 |factorial
	 |combinatorio
	 ;

ciclo:
     WHILE P_A condicion P_C { printf("     WHILE\n");} LL_A bloque LL_C
     | WHILE P_A condicion P_C THEN  { printf("     WHILE THEN ENDWHILE\n");} bloque ENDWHILE
          ;

asignacion: 
          ID ASIG expresion {printf("    ASIGNACION\n");}
	  | ID ASIG_MAS expresion  {printf("    ASIGNACION +=\n");}
	  | ID ASIG_MEN expresion  {printf("    ASIGNACION -=\n");}
	  | ID ASIG_MULT expresion  {printf("    ASIGNACION +=\n");}
	  | ID ASIG_DIV expresion  {printf("    ASIGNACION +=\n");}
	  ;

salida_pantalla:
	WRITE expresion {printf("    SALIDA_PANTALLA\n");}
	;

ingreso_valor:
	READ factor  {printf("    INGRESO_VALOR_READ\n");}
	;

factorial:
	FACT P_A expresion P_C {printf("    FACTORIAL\n");}
	;

combinatorio:
	COMB P_A expresion COMA expresion P_C {printf("    FACTORIAL\n");}	
	;

seleccion: 
    	 IF  P_A condicion P_C LL_A bloque LL_C {printf("     IF\n");}
        | IF P_A condicion P_C THEN bloque ENDIF {printf("     IF THEN END IF\n");}
	| IF P_A condicion P_C LL_A bloque LL_C ELSE LL_A bloque LL_C {printf("     IF con ELSE\n");}	 
;

condicion:
         comparacion 
	 ;

comparacion:
	 expresion OP_MAY_IG expresion
	 |expresion OP_MEN_IG expresion
	 |expresion OP_MEN expresion
	 |expresion OP_MAY expresion
	 |expresion OP_DISTINTO expresion
	   ;

expresion:
         termino
	 |expresion OP_SUMA termino
	 |expresion OP_RESTA termino
 	 ;

termino: 
       factor
       |termino OP_MUL factor
       |termino OP_DIV factor
       ;

factor: 
      ID
      | REAL
      | ENTERO  
      | CADENA
			| P_A expresion P_C
      ;

%%
int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL)
  {
	printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else
  {
	yyparse();
  }
  fclose(yyin);
  return 0;
}


int yyerror(void)
     {
       printf("Syntax Error\n");
	 system ("Pause");
	 exit (1);
     }
	 

	 

	 
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



