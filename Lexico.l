%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include "y.tab.h"

FILE  *yyin;


#define LIM_STR 30
#define LIM_ID 30

int validarLongitud(int);
void mostrarError(char *);

char auxS[100];

%}
%option noyywrap  
%option yylineno 

DIGITO              [0-9]
LETRA               [a-zA-Z]
ID                  {LETRA}({LETRA}|{DIGITO}|_)*
COMENTARIO_INICIO   "***/"
COMENTARIO_FIN      "/***"
COMENTARIOS         {COMENTARIO_INICIO}[^"\n]+{COMENTARIO_FIN}
CADENA              \"[^"\n]+\"
REAL                ({DIGITO}*"."{DIGITO}+)|("."{DIGITO}+)|({DIGITO}+"."{DIGITO}*)
ENTERO              {DIGITO}+
%%

":="            {return ASIG;}
"+="            {return ASIG_MAS;}
"-="            {return ASIG_MEN;}
"*="            {return ASIG_MULT;}
"/="            {return ASIG_DIV;}
"=="            {return OP_IGUAL;}
"!="            {return OP_DISTINTO;}
"+"             {return OP_SUMA;}
"-"             {return OP_RESTA;}
"*"             {return OP_MUL;}
"/"             {return OP_DIV;}
"("             {return P_A;}
")"             {return P_C;}
";"             {return P_Y_C;}
":"             {return DOSPUNTOS;}
","             {return COMA;}
"{"             {return LL_A;}
"}"             {return LL_C;}
"["             {return C_A;}
"]"             {return C_C;}
">"             {return OP_MAY; } 
">="            {return OP_MAY_IG; } 
"<"             {return OP_MEN; } 
"<="            {return OP_MEN_IG; } 
            
"IF"            {return IF;}
"THEN"          {return THEN;}
"ELSE"          {return ELSE;}
"ENDIF"         {return ENDIF;}
"WHILE"         {return WHILE;}
"ENDWHILE"      {return ENDWHILE;}
"AND"           {return AND;}
"OR"            {return OR;}
"NOT"           {return NOT;}
"DEFVAR"        {return DEFVAR;}
"ENDDEF"        {return ENDDEF;}
"FLOAT"         {return FLOAT;}
"STRING"        {return STRING;}
"INT"           {return INTEGER;}
"DISPLAY"       {return DISPLAY;}
"GET"           {return GET;}
"COMB"          {return COMB;}
"FACT"          {return FACT;}
"CONST"         {return CONST;}

{ID}            {   sprintf(auxS,"%s", yytext);
                    if(validarLongitud(ID) == 0) 
                        mostrarError(yytext);
                    yylval.str_val=auxS;
                    return ID;
                }

{ENTERO}        {   if(validarLongitud(ENTERO) == 0) 
                        mostrarError(yytext);
                    sscanf(yytext,"%d",&(yylval.int_val));
                    return ENTERO;
                }
                
{CADENA}        {  if(validarLongitud(CADENA) == 0) 
                        mostrarError(yytext);
                    sprintf(auxS,"%s", yytext);
                    yylval.str_val=auxS; 
                    return CADENA;
                }

{REAL}          {   if(validarLongitud(REAL) == 0) 
                        mostrarError(yytext);
                    sscanf(yytext,"%lf",&(yylval.float_val));
                    return REAL;
                }
                
{COMENTARIOS}

"\n"              
"\t"        
%%

void mostrarError(char *mensaje) {
  printf("ERROR!!!: %s\n", mensaje);
  system ("Pause");
  exit(2);
}

int validarLongitud(int tipo) {
    int entero;
    int real;
    switch(tipo) {
        case ENTERO:
            sscanf(yytext,"%d",&entero);
            if(entero >= 0 && entero <= 65535)
                return 1;
            return 0;
            break;
        case CADENA:
            if(strlen(yytext) <= LIM_STR) 
                return 1;
            return 0;
            break;
        case REAL:
            sscanf(yytext,"%lf",&real);
            double num = atof(yytext);
            double cotaMayor = (3.4)*(exp(38));            
            if(num >= 0 && num <= cotaMayor)
                return 1; 
                        
            return 0;
            break;
        case ID:
            if(strlen(yytext) <= LIM_ID)
                return 1;
            return 0;
            break;
    }
    return 0;
}

 