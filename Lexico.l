%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include "y.tab.h"
FILE  *yyin;
int yylval;
char *yyltext;
char *yytext;

%}
%option noyywrap  
%option yylineno 

DIGITO				[0-9]
LETRA				[a-zA-Z]
ID				{LETRA}({LETRA}|{DIGITO}|_)*
COMENTARIO_INICIO 		"***/"
COMENTARIO_FIN 			"/***"
COMENTARIOS			COMENTARIO_INICIO[^"\n]+COMENTARIO_FIN
CADENA               		\"[^"\n]+\"
REAL              		({DIGITO}*"."{DIGITO}+)|("."{DIGITO}+)
ENTERO               		{DIGITO}+
%%

"="				{return ASIG;}
"+="				{return ASIG_MAS;}
"-="				{return ASIG_MEN;}
"*="				{return ASIG_MULT;}
"/="			        {return ASIG_DIV;}
"=="				{return OP_IGUAL;}
"!="				{return OP_DISTINTO;}
"+"				{return OP_SUMA;}
"-"				{return OP_RESTA;}
"*"				{return OP_MUL;}
"/"				{return OP_DIV;}
"("				{return P_A;}
")"				{return P_C;}
";"				{return P_Y_C;}             
":"         			{return DOSPUNTOS;}
","         			{return COMA;}
"{"         			{return LL_A;}
"}"         			{return LL_C;}
"["         			{return C_A;}
"]"         			{return C_C;}
"***/"        			{return COMENTARIO_INICIO;}
"/***"        			{return COMENTARIO_FIN; }
">" 				{return OP_MAY; } 
">=" 				{return OP_MAY_IG; } 
"<" 				{return OP_MEN; } 
"<=" 				{return OP_MEN_IG; } 
			
"IF"   				{return IF;}
"THEN"   			{return THEN;}
"ELSE"   			{return ELSE;}
"ENDIF"   			{return ENDIF;}
"WHILE"   			{return WHILE;}
"ENDWHILE"			{return ENDWHILE;}
"AND"   			{return AND;}
"OR"   				{return OR;}
"NOT"				{return NOT;}
"DEFVAR"   			{return DEFVAR;}
"ENDDEF"   			{return ENDDEF;}
"FLOAT"   			{return FLOAT;}
"STRING"   			{return STRING;}
"INTEGER"   			{return INTEGER;}
"WRITE"   			{return WRITE;}
"READ"   			{return READ;}
"COMB"                          {return COMB;}
"FACT"                          {return FACT;}

{ID}				{return ID;}
{ENTERO}		{	int num = atoi(yytext); 		
				if(num >= 0 && num <= 65535){
					return ENTERO;
				}
				else	{
					printf("Error. Integer fuera de rango.\n");
					system ("Pause");
					exit(1);
				} 
			}
{CADENA} 			{return CADENA;}
{REAL} 			{	double num = atof(yytext);
				double cotaMayor = (3.4)*(exp(38));			
				if(num >= 0 && num <= cotaMayor){
					return REAL; 
				}
				else{
					printf("Error. Real fuera de rango.\n");
			 		system ("Pause");
					exit(2);
				}
			}

{COMENTARIOS}
"\n"      		
"\t"		
 