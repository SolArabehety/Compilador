
%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "y.tab.h"
/* tercetos.h ya incluye a tabla_simbolos.h */
#include "tercetos.h"
#include "pila.h"

int yystopparser=0;
FILE *yyin;
extern int yylineno;


int tipo;

char* decs[LIM_SIMBOLOS];       // Declaraciones
int decsIndex = 0;              // Indice de declaraciones

void validarIdDeclaracion(char*);
void validarIdExistente(char*);
int yyerror(char*);
void inicializarCompilador();

/* Punteros y pilas para expresiones */
int indExpr, indTerm, indFact;
pila pilaExpr, pilaTerm, pilaFact;

/* Punteros y pilas para factorial y combinatorio */
int indFactorial;
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
%type <int_val> expresion
%type <int_val> termino
%type <int_val> factor
%%

programa:         
    {   printf("Inicia COMPILADOR\n"); inicializarCompilador(); } 
    est_declaracion algoritmo    
    {   guardarTOS();
        imprimirTercetos();
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
                        crearTercetoVariable($3, tipo);
					}
				| ID  
					{  
						validarIdDeclaracion($1);
                        crearTercetoVariable($1, tipo);
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
    ID { validarIdExistente($1); } ASIG expresion           { printf("    ASIGNACION :=\n"); }    
    | ID { validarIdExistente($1); } ASIG_MAS expresion     { printf("    ASIGNACION +=\n"); }
    | ID { validarIdExistente($1); } ASIG_MEN expresion     { printf("    ASIGNACION -=\n"); }
    | ID { validarIdExistente($1); } ASIG_MULT expresion    { printf("    ASIGNACION *=\n"); }
    | ID { validarIdExistente($1); } ASIG_DIV expresion     { printf("    ASIGNACION /=\n"); }
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
    FACT P_A expresion P_C      { printf("    FACTORIAL\n"); generarCodigoFactorial(); }
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
      expresion { apilar(&pilaExpr, indExpr); } OP_SUMA termino  { indExpr = crearTercetoOperacion("+", desapilar(&pilaExpr), indTerm); }
    | expresion { apilar(&pilaExpr, indExpr); } OP_RESTA termino { indExpr = crearTercetoOperacion("-", desapilar(&pilaExpr), indTerm); }
	| termino   { indExpr = indTerm; }
    ;

termino: 
    factor    { indTerm = indFact; }
    | termino { apilar(&pilaTerm, indTerm); } OP_MUL factor { indTerm = crearTercetoOperacion("*", desapilar(&pilaTerm), indFact); }
    | termino { apilar(&pilaTerm, indTerm); } OP_DIV factor { indTerm = crearTercetoOperacion("/", desapilar(&pilaTerm), indFact); }
    ;

factor: 
    P_A expresion P_C   { indFact = indExpr; }
    
	| ID
		{
			validarIdExistente($1);
            indFact = buscarTerceto($1);
		}
    
	| REAL		
		{
			int ind = insertar_REAL_en_Tabla($1);
            indFact = crearTercetoConstanteReal($1);
		}
		
    | ENTERO    
		{	
			int ind = insertar_ENTERO_en_Tabla($1);
            indFact = crearTercetoConstanteEntera($1);
		}
    | CADENA
		{
			int ind = insertar_STRING_en_Tabla($1);
            indFact = crearTercetoConstanteString($1);
		}
    | factorial         { indFact = indFactorial; }
    | combinatorio      { indFact = -1; }
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

/*  Esta funcion es para cualquier cosa que se necesite hacer antes de iniciar con el parsing.
    Hay que hacerlo aca porque en el bloque de arriba de todo no se pueden llamar funciones 
    aparentement. */
void inicializarCompilador() {
    inicializarPila(&pilaExpr);
    inicializarPila(&pilaTerm);
    inicializarPila(&pilaFact);
}

void generarCodigoFactorial() {
    int indFactorial = crearTercetoVariable("@fact", entero);
    int indFactAux = crearTercetoVariable("@fact_aux", entero);
    crearTercetoOperacion("=", indFactAux, indExpr);

    /* Si el número es 1 o 0, directamente resolvemos a 1 y saltamos
        todo el while */
    int indConst1 = crearTercetoConstanteEntera(1);
    int indFactCmp = crearTercetoOperacion("CMP", indFactAux, indConst1);
    int indBranch1 = crearTercetoBranch("BGT", 0);
    crearTercetoOperacion("=", indFactorial, indConst1);
    int indBranch2 = crearTercetoBranch("BI", 0);

    /* Inicializamos el factorial en 0 */
    int indConst0 = crearTercetoConstanteEntera(0);
    crearTercetoOperacion("=", indFactorial, indConst0);
    modificarSaltoTerceto(indBranch1, indConst0);

    /* Loop para multiplicar sucesivamente */
    indFactCmp = crearTercetoOperacion("CMP", indFactAux, indConst1);
    int indBranch3 = crearTercetoBranch("BLE", 0);
    int indFactResta = crearTercetoOperacion("-", indFactAux, indConst1);
    int indFactMult = crearTercetoOperacion("*", indFactAux, indFactResta);
    int indFactSum = crearTercetoOperacion("+", indFactorial, indFactMult);
    crearTercetoOperacion("=", indFactAux, indFactResta);
    crearTercetoOperacion("=", indFactorial, indFactSum);
    int indFinal = crearTercetoBranch("BI", indFactCmp);
    printf("indFinal: %d\n", indFinal);
    /* Seteamos los saltos para los branches que quedaron colgados */
    modificarSaltoTerceto(indBranch2, indFinal + 1);
    modificarSaltoTerceto(indBranch3, indFinal + 1);
}