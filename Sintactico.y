
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
indice indExpr, indTerm, indFact, indComp, indIf;
pila pilaExpr, pilaTerm, pilaFact, pilaCond;

/* Punteros y pilas para factorial y combinatorio */
indice indFactorial;

/* Punteros auxiliares */
indice indExprAux; 

char* devolverSaltoCondicional(char*);
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
    DEFVAR  { printf("        DECLARACIONES\n"); } declaraciones ENDDEF { printf("    Fin de las Declaraciones\n"); printf("Regla 1\n");}
    ;

        
declaraciones:   

				declaraciones declaracion {printf("Regla 2\n");}		
			 |	declaracion {printf("Regla 3\n");}	
			;
	


declaracion:
    tipo_variable DOSPUNTOS lista_declaracion   {printf("    DECLARACION\n"); printf("Regla 4\n");		}
    ;

tipo_variable:
		STRING		{ tipo = string;  printf("Regla 5\n");} 
	| 	INTEGER 	{ tipo = entero;  printf("Regla 6\n");}
	|	FLOAT		{ tipo = real;  printf("Regla 7\n");} 



lista_declaracion:  
				lista_declaracion P_Y_C  ID  
					{   printf("Regla 8\n");
						validarIdDeclaracion($3);
                        cargarVariable($3, tipo);
					}
				| ID  
					{   printf("Regla 9\n");
						validarIdDeclaracion($1);
                        cargarVariable($1, tipo);
					};

 
 
algoritmo: 
    { printf("      COMIENZO de BLOQUES\n"); } bloque{ printf("Regla 10\n");}
    ;

bloque:  
	bloque sentencia{ printf("Regla 11\n");}
    | sentencia{ printf("Regla 12\n");}
    ;

sentencia:
    ciclo					{ printf("Regla 13\n");}
    | declaracion_constante	{ printf("Regla 14\n");}
    | seleccion  			{ printf("Regla 15\n");}
    | asignacion			{ printf("Regla 16\n");}
    | salida_pantalla		{ printf("Regla 17\n");}
    | ingreso_valor     	{ printf("Regla 18\n");}
    | factorial				{ printf("Regla 19\n");}
    | combinatorio			{ printf("Regla 20\n");}
    ;

ciclo:
    WHILE P_A condicion P_C         { printf("     WHILE\n"); } LL_A bloque LL_C{ printf("Regla 21\n");}
    | WHILE P_A condicion P_C THEN  { printf("     WHILE THEN ENDWHILE\n"); } bloque ENDWHILE{ printf("Regla 22\n");}
    ;

declaracion_constante:
    CONST ID { validarIdDeclaracion($2); strcpy(nombreToken, $2); } ASIG expresion{ printf("Regla 23\n");}
    ;

// TEMA ESPECIAL: ASIGNACIONES ESPECIALES 
asignacion: 
    ID ASIG expresion           { printf("    ASIGNACION :=\n"); generarCodigoAsignacion($1); 		printf("Regla 24\n");}    
    | ID ASIG_MAS expresion     { printf("    ASIGNACION +=\n"); generarCodigoAsignacionEsp($1, "+"); printf("Regla 25\n");}
    | ID ASIG_MEN expresion     { printf("    ASIGNACION -=\n"); generarCodigoAsignacionEsp($1, "-"); printf("Regla 26\n");}
    | ID ASIG_MULT expresion    { printf("    ASIGNACION *=\n"); generarCodigoAsignacionEsp($1, "*"); printf("Regla 27\n");}
    | ID ASIG_DIV expresion     { printf("    ASIGNACION /=\n"); generarCodigoAsignacionEsp($1, "/"); printf("Regla 28\n");}
    ;

salida_pantalla:
    DISPLAY expresion             
		{ 
			printf("    SALIDA_PANTALLA\n"); printf("Regla 29\n"); 
		}
    ;

ingreso_valor:
    GET factor                 { printf("    INGRESO_VALOR_GET\n"); printf("Regla 30\n");}
    ;

factorial:
    FACT P_A expresion P_C      { printf("Regla 31\n"); printf("    FACTORIAL\n"); generarCodigoFactorial(); }
    ;

combinatorio:
    COMB P_A expresion COMA expresion P_C { printf("    COMBINATORIO\n");; printf("Regla 32\n");}       
    ;

seleccion: 
    IF  P_A condicion P_C
		LL_A bloque LL_C  { printf("     IF\n");printf("Regla 33\n"); 
							modificarSaltoTerceto(desapilar(&pilaCond), indTercetos);
						}
    
	| IF P_A condicion P_C 
		THEN bloque ENDIF  {	
							printf("  	IF THEN ENDIF\n");	printf("Regla 34\n");
							modificarSaltoTerceto(desapilar(&pilaCond), indTercetos);
						}
						
    | IF P_A condicion P_C	
		LL_A bloque LL_C  	{	
								modificarSaltoTerceto(desapilar(&pilaCond), indTercetos+1);
								apilar(&pilaCond, crearTercetoBranch("JMP",0) );
							}
		ELSE LL_A bloque LL_C  { 
								printf("     IF con ELSE\n"); printf("Regla 35\n");
								modificarSaltoTerceto(desapilar(&pilaCond), indTercetos);
			}     
    ;

condicion:
    comparacion 			{printf("Regla 36\n");}
	| comparacion_doble 	{printf("Regla 37\n");}
	| comparacion_negada	{printf("Regla 38\n");}
    ;
	
comparacion_negada:
	NOT P_A comparacion P_C 	{	printf("    NOT\n");  printf("Regla 39\n"); 
									negarTerceto(indTercetos-1);
									//apilar(&pilaCond, negarTerceto(desapilar(&pilaExpr))); 
								}
	| NOT P_A comparacion_doble P_C 	{ printf("    NOT\n");  printf("Regla 40\n");}

comparacion_doble:
	P_A comparacion P_C AND P_A comparacion P_C 	{ printf("    AND\n");	printf("Regla 41\n"); }
	| P_A comparacion P_C OR P_A comparacion P_C 	{ printf("    OR\n"); 	printf("Regla 42\n"); }
	;

comparacion:
    expresion	{ indExprAux = indExpr; } OP_MAY_IG expresion	{	printf("Regla 43\n");	
																	indComp =  crearTercetoOperacion("CMP", indExprAux, indExpr); 
																	apilar(&pilaCond, crearTercetoBranch(devolverSaltoCondicional(">="),0) );
																}	
    | expresion	{ indExprAux = indExpr; } OP_MEN_IG expresion	{	printf("Regla 44\n");
																	indComp =  crearTercetoOperacion("CMP", indExprAux, indExpr); 
																	apilar(&pilaCond, crearTercetoBranch(devolverSaltoCondicional("<="),0) );
																}	
    | expresion	{ indExprAux = indExpr; } OP_MEN expresion		{	printf("Regla 45\n");	
																	indComp =  crearTercetoOperacion("CMP", indExprAux, indExpr); 
																	apilar(&pilaCond, crearTercetoBranch(devolverSaltoCondicional("<"),0) );
																}	
    | expresion { indExprAux = indExpr; } OP_MAY expresion		{	printf("Regla 46\n");	
																	indComp =  crearTercetoOperacion("CMP", indExprAux, indExpr); 
																	apilar(&pilaCond, crearTercetoBranch(devolverSaltoCondicional(">"),0) );
																}	
    | expresion	{ indExprAux = indExpr; } OP_DISTINTO expresion	{	printf("Regla 47a\n");	
																	indComp =  crearTercetoOperacion("CMP", indExprAux, indExpr); 
																	apilar(&pilaCond, crearTercetoBranch(devolverSaltoCondicional("!="),0) );
																}	
	| expresion	{ indExprAux = indExpr; } OP_IGUAL expresion	{	printf("Regla 47b\n");	
																	indComp =  crearTercetoOperacion("CMP", indExprAux, indExpr); 
																	apilar(&pilaCond, crearTercetoBranch(devolverSaltoCondicional("=="),0) );
																}	
    ;

expresion:
      expresion { apilar(&pilaExpr, indExpr); } OP_SUMA termino  { indExpr = crearTercetoOperacion("+", desapilar(&pilaExpr), indTerm);  printf("Regla 48\n");}
    | expresion { apilar(&pilaExpr, indExpr); } OP_RESTA termino { indExpr = crearTercetoOperacion("-", desapilar(&pilaExpr), indTerm);  printf("Regla 49\n");}
	| termino   { indExpr = indTerm;  printf("Regla 50\n"); }
    ;

termino: 
    factor    { indTerm = indFact; printf("Regla 51\n");}
    | termino { apilar(&pilaTerm, indTerm); } OP_MUL factor { indTerm = crearTercetoOperacion("*", desapilar(&pilaTerm), indFact); printf("Regla 52\n");}
    | termino { apilar(&pilaTerm, indTerm); } OP_DIV factor { indTerm = crearTercetoOperacion("/", desapilar(&pilaTerm), indFact); printf("Regla 53\n");}
    ;

factor: 
    P_A expresion P_C   { indFact = indExpr; printf("Regla 54\n");}
    
	| ID
		{	printf("Regla 55\n");
			validarIdExistente($1);
            /* La variable ya va a estar cargada en la tabla, esto va a devolver su índice. */
            indFact = buscarIndiceSimbolo($1);
		}
	| REAL	    { indFact = cargarConstanteReal($1); 	printf("Regla 56\n");}	
    | ENTERO    { indFact = cargarConstanteEntera($1); 	printf("Regla 57\n");}
    | CADENA    { indFact = cargarConstanteString($1); 	printf("Regla 58\n");}
    | factorial         { indFact = indFactorial;		printf("Regla 59\n"); }
    | combinatorio      { printf("Regla 60\n");
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
	inicializarPila(&pilaCond);
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


/**	
	Retorna la instruccion assembler correspondiente al caracter recibido
	-> Cuando empecemos a trabajar con assembler, hay que mover esta funcion a la clase correspondiente.
**/
char* devolverSaltoCondicional(char* comparacion){
	if(strcmp(comparacion, ">=") == 0)
		return "JNB";
	if(strcmp(comparacion, ">") == 0)
		return "JNBE";
	if(strcmp(comparacion, "<=") == 0)
		return "JNA";
	if(strcmp(comparacion, "<") == 0)
		return "JNAE"; 
	if(strcmp(comparacion, "!=") == 0)
		return "JNE";
	if(strcmp(comparacion, "==") == 0)
		return "JE";
	
	return NULL;
}
