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
    fprintf(f, ".MODEL LARGE\n");
    fprintf(f, ".386\n");
    fprintf(f, ".STACK 200h\n");
    fprintf(f, "\n");
}

void generaCuerpo(FILE* f) {
    fprintf(f, ".CODE\n\n");
    fprintf(f, "MOVE EAX, @DATA\n");
    fprintf(f, "MOVE DS, EAX\n");
    fprintf(f, "MOVE ES, EAX\n\n");

    fprintf(f, ";Seccion de codigo\n");
    generaPrograma(f);

    fprintf(f, "MOVE EAX, 4C00h\n");
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
        /* Imprime nombre */
        fprintf(f, "%s ", TOS[i].nombre);
        
        /* Lo siguiente depende del tipo de símbolo */
        switch(TOS[i].tipo) {
            case string:
                fprintf(f, "db %d dup (?),\"$\"", LIM_STR);
                break;
            case entero:
                fprintf(f, "dd ?");
                break;
            case real:
                fprintf(f, "dd ?");
                break;
            case constString:
                fprintf(f, "db \"%s\", \"$\", %d dup (?)", TOS[i].valor, LIM_STR);
                break;
            case constEntero:
                /* Hay que convertirlo a float */
                val = atof(TOS[i].valor);
                fprintf(f, "dd %f", val);
                break;
            case constReal:
                val = atof(TOS[i].valor);
                fprintf(f, "dd %f", val);
                break;
        }
        fprintf(f, "\n");
    }

    fprintf(f, "\n");
    fprintf(f, "END;\n");
}

void generaPrograma (FILE* f) {
    int i;

    for (i = 0; i < indTercetos; i++) {
        char buffer[100];
        terceto t = tercetos[i];
        if (DEBUG) fprintf(f, ";Terceto %d\n", i + 1);
        
        switch(t.tipoTerc) {
            case esAsignacion:
                /* A implementar */
                break;
            case esSuma:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "FLD %s \nFLD %s \nFADD \nFSTP %s",
                 resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;
            case esResta:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "FLD %s \nFLD %s \nFSUB \nFSTP %s",
                 resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;
            case esMultiplicacion:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "FLD %s \nFLD %s \nFMULT \nFSTP %s",
                 resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;
            case esDivision:
                sprintf(buffer, "@aux%d", i + 1);
                fprintf(f, "FLD %s \nFLD %s \nFDIV \nFSTP %s",
                 resolverElemento(t.elementos[1]), resolverElemento(t.elementos[2]), buffer);
                cargarVariable(buffer, real);
                break;
            case esComparacion:
                /* A implementar */
                break;
            case esSalto:
                /* A implementar */
                break;
            case esEtiqueta:
                /* A implementar */
                break;
            case esGet:
                /* A implementar */
                break;
            case esDisplay:
                /* A implementar */
                break;
            default:
                break;
        }
        fprintf(f, "\n");
    }
}

/*  Esta función toma un índice y:
    - Si es un terceto, devuelve su etiqueta aux correspondiente.
    - Si es un símbolo, devuelve su nombre en la tabla. */
const char* resolverElemento (elemento e) {
    char buffer[100];

    if (e.tipo == entero) {
        sprintf(buffer, "@aux%d", e.valor.ind);
    } else if (e.tipo == string) { /* Es un símbolo */
        sprintf(buffer, "%s", e.valor.cad);
    } else {
        /*  Ojo que esto no debería pasar, si ocurre es una señal
            de que hay algo mal en la generación de assembler. */
        sprintf(buffer, "NULL");
    }

    return strdup(buffer);
}
