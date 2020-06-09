#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "tercetos.h"

void generaAssembler();
void generaCabecera(FILE*);
void generaTOS(FILE*);
void generaCuerpo(FILE*);
void generaFinal(FILE*);

void generaAssembler() {
    FILE *f;

    if ((f = fopen("final.asm", "w")) == NULL) {
        printf("No se puede crear el archivo final.asm.");
        exit(1);
    }

    generaCabecera(f);
    generaTOS(f);
    generaCuerpo(f);
    generaFinal(f);
    
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
}

void generaCuerpo(FILE* f) {
    fprintf(f, ".CODE\n\n");
    fprintf(f, "MOVE EAX, @DATA\n");
    fprintf(f, "MOVE DS, EAX\n");
    fprintf(f, "MOVE ES, EAX\n\n");
    fprintf(f, ";Seccion de codigo\n");
    fprintf(f, "\n");
}

void generaFinal(FILE* f) {
    fprintf(f, "MOVE EAX, 4C00h\n");
    fprintf(f, "INT 21h\n\n");
    fprintf(f, "END;\n");
}
