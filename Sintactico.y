
%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "tercetos.h"
#include "y.tab.h"
/* tercetos.h ya incluye a tabla_simbolos.h */

int yystopparser=0;
FILE *yyin;

int tipo;

char* decs[LIM_SIMBOLOS];       // Declaraciones
int decsIndex = 0;              // Indice de declaraciones

/* Punteros y pilas para expresiones */
indice indExpr, indTerm, indFact;
pila pilaExpr, pilaTerm, pilaFact;

/* Punteros y pilas para factorial y combinatorio */
indice indFactorial;

void validarIdDeclaracion(const char*);
void validarIdExistente(const char*);
int yyerror(char*);
void inicializarCompilador();
void generarCodigoAsignacion(const char*);
void generarCodigoAsignacionEsp(const char*, const char*);
void generarCodigoFactorial();
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
		STRING		{ tipo = string; } 
	| 	INTEGER 	{ tipo = entero; }
	|	FLOAT		{ tipo = real; } 



lista_declaracion:  
				lista_declaracion P_Y_C  ID  
					{  
						validarIdDeclaracion($3);
                        cargarVariable($3, tipo);
					}
				| ID  
					{  
						validarIdDeclaracion($1);
                        cargarVariable($1, tipo);
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
    ID ASIG expresion           { printf("    ASIGNACION :=\n"); generarCodigoAsignacion($1); }    
    | ID ASIG_MAS expresion     { printf("    ASIGNACION +=\n"); generarCodigoAsignacionEsp($1, "+"); }
    | ID ASIG_MEN expresion     { printf("    ASIGNACION -=\n"); generarCodigoAsignacionEsp($1, "-"); }
    | ID ASIG_MULT expresion    { printf("    ASIGNACION *=\n"); generarCodigoAsignacionEsp($1, "*"); }
    | ID ASIG_DIV expresion     { printf("    ASIGNACION /=\n"); generarCodigoAsignacionEsp($1, "/"); }
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
    COMB P_A expresion COMA expresion P_C { printf("    COMBINATORIO\n"); }    
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
            /* La variable ya va a estar cargada en la tabla, esto va a devolver su índice. */
            indFact = buscarIndiceSimbolo($1);
		}
	| REAL	    { indFact = cargarConstanteReal($1); }	
    | ENTERO    { indFact = cargarConstanteEntera($1); }
    | CADENA    { indFact = cargarConstanteString($1); }
    | factorial         { indFact = indFactorial; }
    | combinatorio      { 
            /*  Esto acá es cualquier cosa simplemente porque combinatorio no está 
                implementado todavía */
            indFact = cargarConstanteEntera(999); 
        }
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

void validarIdDeclaracion(const char* id) {
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

void validarIdExistente(const char* id) {
    int i;

    for(i = 0; i < decsIndex; i++) {
        if(strcmp(decs[i], id) == 0) {
            return;
        }
    }

    printf("\nError en la linea %d: El ID '%s' no ha sido declarado.\n", yylineno, id);
    exit(1);
}

/*  Esta función es para cualquier cosa que se necesite hacer antes de iniciar con el parsing.
    Hay que hacerlo aca porque en el bloque de arriba de todo no se pueden llamar funciones 
    aparentement. */
void inicializarCompilador() {
    inicializarPila(&pilaExpr);
    inicializarPila(&pilaTerm);
    inicializarPila(&pilaFact);
}

void generarCodigoFactorial() {
    indFactorial = cargarVariable("@fact", entero);
    indice indFactAux = cargarVariable("@factAux", entero);
    crearTercetoAsignacion(indFactAux, indExpr);

    /* Inicializamos el factorial en 1 */
    indice indConst1 = cargarConstanteEntera(1);
    crearTercetoAsignacion(indFactorial, indConst1);

    /* Loop para multiplicar sucesivamente */
    indice indFactCmp = crearTercetoOperacion("CMP", indFactAux, indConst1);
    indice indBranch = crearTercetoBranch("BLE", 0);
    indice indFactMult = crearTercetoOperacion("*", indFactorial, indFactAux);
    crearTercetoAsignacion(indFactorial, indFactMult);
    indice indFactResta = crearTercetoOperacion("-", indFactAux, indConst1);
    crearTercetoAsignacion(indFactAux, indFactResta);
    indice indFinal = crearTercetoBranch("BI", indFactCmp.num);

    /* Seteamos los saltos para los branches que quedaron colgados */
    modificarSaltoTerceto(indBranch, indFinal.num + 1);
}

void generarCodigoAsignacion(const char* id) {
    validarIdExistente(id);
    crearTercetoAsignacion(buscarIndiceSimbolo(id), indExpr);
}

void generarCodigoAsignacionEsp(const char* id, const char* op) {
    validarIdExistente(id);
    indice indSimbolo = buscarIndiceSimbolo(id);
    indice indOp = crearTercetoOperacion(op, indSimbolo, indExpr);
    crearTercetoAsignacion(indSimbolo, indOp);
}
