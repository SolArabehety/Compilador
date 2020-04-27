
%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "y.tab.h"
#include "tabla_simbolos.h"


int yystopparser=0;
FILE *yyin;
extern int yylineno;


int tipo;

char* decs[LIM_SIMBOLOS];       // Declaraciones
int decsIndex = 0;              // Indice de declaraciones

void validarIdDeclaracion(char*);
void validarIdExistente(char*);
int yyerror(char*);


%}

%error-verbose

%union {
    int int_val;
    double float_val;
    char *str_val;
}

%token ID COMENTARIO_INICIO COMENTARIO_FIN COMENTARIOS CADENA REAL ENTERO 
%token OP_SUMA OP_RESTA OP_MUL OP_DIV ASIG P_A P_C LL_A LL_C P_Y_C COMA OP_MAY_IG OP_MEN_IG 
%token OP_MAY OP_MEN OP_DISTINTO C_A C_C DOSPUNTOS OP_IGUAL ASIG_MAS ASIG_MEN ASIG_MULT ASIG_DIV
%token DEFVAR ENDDEF DISPLAY IF THEN ELSE ENDIF WHILE ENDWHILE FLOAT INTEGER AND OR NOT STRING GET COMB FACT CONST

%type <str_val> ID CADENA
%type <int_val> ENTERO
%type <float_val> REAL
%%

programa:         
    {   printf("Inicia COMPILADOR\n"); } 
    est_declaracion algoritmo    
    {   guardarTOS();
        printf("Fin COMPILADOR ok\n");
    };

est_declaracion:
    DEFVAR  { printf("        DECLARACIONES\n"); } declaraciones ENDDEF { printf("    Fin de las Declaraciones\n"); }
    ;

        
declaraciones:   

				declaraciones declaracion
			 |	declaracion
			;
	


declaracion:
    tipo_variable DOSPUNTOS lista_declaracion  {printf("    DECLARACION\n");}
    ;

tipo_variable:
		STRING		{tipo = 1; } 
	| 	INTEGER 	{tipo = 2;}
	|	FLOAT		{tipo = 3;} 



lista_declaracion:  
				lista_declaracion P_Y_C  ID  
					{  
						validarIdDeclaracion($3);
						insertar_ID_en_Tabla($<str_val>$, tipo);
					}
				| ID  
					{  
						validarIdDeclaracion($1);
						insertar_ID_en_Tabla($<str_val>$, tipo);
					};

 
 
algoritmo: 
    { printf("      COMIENZO de BLOQUES\n"); } bloque
    ;

bloque:  
	bloque sentencia
    | sentencia
    ;

sentencia:
    ciclo
    | declaracion_constante
    | seleccion  
    | asignacion
    | salida_pantalla
    | ingreso_valor      
    | factorial
    | combinatorio
    ;

ciclo:
    WHILE P_A condicion P_C         { printf("     WHILE\n"); } LL_A bloque LL_C
    | WHILE P_A condicion P_C THEN  { printf("     WHILE THEN ENDWHILE\n"); } bloque ENDWHILE
    ;

declaracion_constante:
    CONST ID { validarIdDeclaracion($2); strcpy(nombreToken, $2); } ASIG expresion
    ;

// TEMA ESPECIAL: ASIGNACIONES ESPECIALES 
asignacion: 
    ID { validarIdExistente($1); } ASIG expresion        
    | ID { validarIdExistente($1); } ASIG_MAS expresion     { printf("    ASIGNACION +=\n"); }
    | ID { validarIdExistente($1); } ASIG_MEN expresion     { printf("    ASIGNACION -=\n"); }
    | ID { validarIdExistente($1); } ASIG_MULT expresion    { printf("    ASIGNACION +=\n"); }
    | ID { validarIdExistente($1); } ASIG_DIV expresion     { printf("    ASIGNACION +=\n"); }
    ;

salida_pantalla:
    DISPLAY expresion             
		{ 
			printf("    SALIDA_PANTALLA\n"); 
		}
    ;

ingreso_valor:
    GET factor                 { printf("    INGRESO_VALOR_GET\n"); }
    ;

factorial:
    FACT P_A expresion P_C      { printf("    FACTORIAL\n"); }
    ;

combinatorio:
    COMB P_A expresion COMA expresion P_C { printf("    FACTORIAL COMBINATORIO\n"); }    
    ;

seleccion: 
    IF  P_A condicion P_C LL_A bloque LL_C                          { printf("     IF\n"); }
    | IF P_A condicion P_C THEN bloque ENDIF                        { printf("     IF THEN END IF\n"); }
    | IF P_A condicion P_C LL_A bloque LL_C ELSE LL_A bloque LL_C   { printf("     IF con ELSE\n"); }     
    ;

condicion:
    comparacion
	| comparacion_doble
	| comparacion_negada
    ;
	
comparacion_negada:
	NOT P_A comparacion P_C 	{ printf("    NOT\n"); }
	| NOT P_A comparacion_doble P_C 	{ printf("    NOT\n"); }

comparacion_doble:
	P_A comparacion P_C AND P_A comparacion P_C 	{ printf("    AND\n"); }
	| P_A comparacion P_C OR P_A comparacion P_C 	{ printf("    OR\n"); }
	;

comparacion:
    expresion OP_MAY_IG expresion		
    | expresion OP_MEN_IG expresion		
    | expresion OP_MEN expresion			
    | expresion OP_MAY expresion		
    | expresion OP_DISTINTO expresion
    ;

expresion:
      expresion OP_SUMA termino
    | expresion OP_RESTA termino
	| termino
    ;

termino: 
    factor
    | termino OP_MUL factor
    | termino OP_DIV factor
    ;

factor: 
    P_A expresion P_C
    
	| ID
		{
			validarIdExistente($1);
		}
    
	| REAL		
		{
			insertar_REAL_en_Tabla($1);
		}
		
    | ENTERO    
		{	
			insertar_ENTERO_en_Tabla($1);
		}
    | CADENA
		{
			insertar_STRING_en_Tabla($1);
		}
    | factorial
    | combinatorio
    ;

%%
int main(int argc,char *argv[]) {
    if ((yyin = fopen(argv[1], "rt")) == NULL) {
        printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
    } else {
        yyparse();
    }

    fclose(yyin);
    return 0;
}

int yyerror(char* mensaje_error) {
    /*  Ojo con esto. Según leí en el manual de Bison, el mensaje de error puede
        ser que no sea 100% preciso. De un modo u otro, si aparece un error entonces
        es porque lo hubo; lo que no garantiza que el mensaje diga ciertamente qué
        fue lo que produjo el error. */
    printf("\nError en la linea %d: %s\n", yylineno, mensaje_error);
    system ("Pause");
    exit (1);
}

void validarIdDeclaracion(char* id) {
    int i;

    for(i = 0; i < decsIndex; i++) {
        if(strcmp(decs[i], id) == 0) {
            printf("\nError en la linea %d: El ID '%s' ya ha sido declarado.\n", yylineno, id);
            exit(1);
        }
    }

    decs[decsIndex] = strdup(id);
    decsIndex++;
}

void validarIdExistente(char* id) {
    int i;

    for(i = 0; i < decsIndex; i++) {
        if(strcmp(decs[i], id) == 0) {
            return;
        }
    }

    printf("\nError en la linea %d: El ID '%s' no ha sido declarado.\n", yylineno, id);
    exit(1);
}
