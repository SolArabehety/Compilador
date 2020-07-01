#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "tercetos.h"

void generaAssembler();
void generaCabecera(FILE*);
void generaTOS(FILE*);
void generaCuerpo(FILE*);
void generaPrograma(FILE*);
const char* resolverElemento(elemento);

void generaAssembler() {
    FILE *f;

    if ((f = fopen("final.asm", "w")) == NULL) {
        printf("No se puede crear el archivo final.asm.");
        exit(1);
    }

    generaCabecera(f);
    generaCuerpo(f);
    generaTOS(f);
    
    if (fclose(f) != 0) {
        printf("No se puede CERRAR el archivo final.asm.");
        exit(1);
    }
    printf("Generado archivo final.asm\n");
}

void generaCabecera(FILE* f) {
    fprintf(f, "include macros.asm\n");
    fprintf(f, "include macros2.asm\n");
    fprintf(f, "include number.asm\n\n");
    fprintf(f, ".MODEL LARGE\n");
    fprintf(f, ".386\n");
    fprintf(f, ".STACK 200h\n");
    fprintf(f, "\n");
}

void generaCuerpo(FILE* f) {
    fprintf(f, ".CODE\n\n");
    fprintf(f, "MAIN:\n\n");
    fprintf(f, "MOV AX, @DATA\n");
    fprintf(f, "MOV DS, AX\n");
    fprintf(f, "MOV ES, AX\n\n");

    fprintf(f, ";Seccion de codigo\n");
    generaPrograma(f);

    fprintf(f, "\n");
    fprintf(f, "MOV AX, 4C00h\n");
    fprintf(f, "INT 21h\n\n");
    fprintf(f, "\n");
}

void generaTOS(FILE* f) {
    int i;
    fprintf(f, ".DATA\n");
    fprintf(f, ";Tabla de simbolos\n\n");

    for (i = 0; i< TOStop; i++)
    {
        float val;
        
        switch(TOS[i].tipo) {
            case string:
                fprintf(f, "%s ", TOS[i].nombre);
                fprintf(f, "db %d dup (?), \"$\"", LIM_STR);
                break;
            case entero:
                fprintf(f, "%s ", TOS[i].nombre);
                fprintf(f, "dd ?");
                break;
            case real:
                fprintf(f, "%s ", TOS[i].nombre);
                fprintf(f, "dd ?");
                break;
            case constString:
                /*  En el caso de const string, el nombre de la variable en assembler se genera
                    con str + num de símbolo, esto hay que hacerlo porque al turbo assembler
                    no le estaban gustando los nombres de la tabla. */
                fprintf(f, "@str%d ", i);
                fprintf(f, "db \"%s\", \"$\", %d dup (?)", TOS[i].valor, LIM_STR);
                break;
            case constEntero:
                fprintf(f, "%s ", TOS[i].nombre);
                /* Hay que convertirlo a float */
                val = atof(TOS[i].valor);
                fprintf(f, "dd %f", val);
                break;
            case constReal:
                fprintf(f, "%s ", TOS[i].nombre);
                val = atof(TOS[i].valor);
                fprintf(f, "dd %f", val);
                break;
        }
        fprintf(f, "\n");
    }

    fprintf(f, "\n");
    fprintf(f, "END MAIN\n");
}

void generaPrograma (FILE* f) {
    int i;

    for (i = 0; i < indTercetos; i++) {
        char buffer[100];
        terceto t = tercetos[i];
        char* salto, *etiqueta, *str, *strSimbolo;
        int indEtiqueta, indSimbolo;
        tipoValor tipoSimbolo;
        if (DEBUG) fprintf(f, "\n;Terceto %d\n", i + 1);
        
        switch(t.tipoTerc) {
            case esAsignacion:
                strSimbolo = t.elementos[1].valor.cad;
                indSimbolo = buscarNombreEnTS(strSimbolo);
                tipoSimbolo = TOS[indSimbolo].tipo;

                if(tipoSimbolo == string) {
                    /*  Si la asignación es a una variable de tipo string
                        la asignación se realiza de una forma especial */
                    fprintf(f, "\tMOV  SI, OFFSET @str%d \n", buscarNombreEnTS(t.elementos[2].valor.cad));
                    fprintf(f, "\tMOV  DI, OFFSET %s \n", resolverElemento(t.elementos[1]));
                    fprintf(f, "\tSTRCPY");
                } else {
                    fprintf(f, "\tFLD %s \n\tFSTP %s",
                        resolverElemento(t.elementos[2]), resolverElemento(t.elementos[1]));
                }
                /* No genera etiqueta aux la asignacion */
                break;

            case esSuma:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "\tFLD %s \n\tFLD %s \n\tFADD \n\tFSTP %s",
                    resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;

            case esResta:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "\tFLD %s \n\tFLD %s \n\tFSUB \n\tFSTP %s",
                    resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;

            case esMultiplicacion:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "\tFLD %s \n\tFLD %s \n\tFMULT \n\tFSTP %s",
                    resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;

            case esDivision:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "\tFLD %s \n\tFLD %s \n\tFDIV \n\tFSTP %s",
                    resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;

            case esComparacion:
                fprintf(f, "\tFLD %s \n\tFCOMP %s \n\tFSTSW AX \n\tSAHF",
                    resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]));
                break;

            case esSalto:
                /*  Acá no hay que usar resolverElemento porque nos devolvería un aux en lugar
                    de la etiqueta. Hay que generarla a mano */
                salto = t.elementos[0].valor.cad;
                indEtiqueta = t.elementos[1].valor.ind;
                etiqueta = tercetos[indEtiqueta].elementos[0].valor.cad;
                fprintf(f, "\t%s %s", salto, etiqueta);
                break;

            case esEtiqueta:
                /*  Simplemente hay que poner el nombre del tag (el primer elemento del terceto)
                    en forma de etiqueta */
                fprintf(f, "\n%s:", t.elementos[0].valor.cad);
                break;

            case esGet:
                strSimbolo = t.elementos[1].valor.cad;
                indSimbolo = buscarNombreEnTS(strSimbolo);
                tipoSimbolo = TOS[indSimbolo].tipo;

                switch(tipoSimbolo) {
                    case string:
                        fprintf(f, "\tgetString %s", resolverElemento(t.elementos[1]));
                        break;
                    case entero:
                    case real:
                        fprintf(f, "\tGetFloat %s", resolverElemento(t.elementos[1]));
                        break;
                    default:
                        printf("\nError al generar Get de: %s", resolverElemento(t.elementos[1]));
                }
                break;

            case esDisplay:
                strSimbolo = t.elementos[1].valor.cad;
                indSimbolo = buscarNombreEnTS(strSimbolo);
                tipoSimbolo = TOS[indSimbolo].tipo;

                switch(tipoSimbolo) {
                    case string:
                        fprintf(f, "\tdisplayString %s\n", resolverElemento(t.elementos[1]));
                        fprintf(f, "\tnewLine");
                        break;
                    case constString:
                        fprintf(f, "\tdisplayString @str%d\n", indSimbolo);
                        fprintf(f, "\tnewLine");
                        break;
                    case entero:
                    case constEntero:
                    case real:
                    case constReal:
                        fprintf(f, "\tDisplayFloat %s\n", resolverElemento(t.elementos[1]));
                        fprintf(f, "\tnewLine");
                        break;
                    default:
                        printf("\nError al generar Display de: %s", resolverElemento(t.elementos[1]));
                }
                break;

            default:
                break;
        }
        fprintf(f, "\n");
    }
}

/*  Esta función toma un elemento de terceto y:
    - Si es un entero, es un índice a terceto por lo que
    devuelve su etiqueta aux correspondiente.
    - Si es una string, es un símbolo y devuelve la
    cadena tal como es. */
const char* resolverElemento (elemento e) {
    char buffer[100];

    if (e.tipo == entero) {
        sprintf(buffer, "@aux%d", e.valor.ind + 1);
    } else if (e.tipo == string) { /* Es un símbolo */
        sprintf(buffer, "%s", e.valor.cad);
    } else {
        /*  Ojo que esto no debería pasar, si ocurre es una señal
            de que hay algo mal en la generación de assembler. */
        sprintf(buffer, "NULL");
    }

    return strdup(buffer);
}
