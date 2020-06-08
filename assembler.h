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
    int i, j;

    if ((f = fopen("final.asm", "w")) == NULL) {
        printf("No se puede crear el archivo final.asm.");
        exit(1);
    }

    generaCabecera(f);
    generaTOS(f);
    generaCuerpo(f);
    generaFinal(f);

    /*fprintf(gci, "\n--- LISTA DE TERCETOS ---\n\n");
    for (i = 0; i < indTercetos; i++) {
        terceto t = tercetos[i];
        fprintf(gci, "%d: (", i + 1);

        for (j = 0; j < 3; j++) {
            elemento e = t.elementos[j];

            switch (e.tipo) {
                case string:
                    fprintf(gci, "%s", e.valor.cad);
                    break;
                case entero:
                    fprintf(gci, "[%d]", e.valor.ind + 1);
                    break;
                default:
                    fprintf(gci, "_");
            }

            if (j < 2) {
                fprintf(gci, ", ");
            } 
        }
        fprintf(gci, ")");
        if (DEBUG) fprintf(gci, "   %s   %s", nombreTiposTercetos[t.tipoTerc], nombreTiposVal[t.tipoVal]);
        fprintf(gci, "\n");
    }
    fprintf(gci,"\n--- LISTA DE TERCETOS ---\n");*/
    
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
    fprintf(f, ".DATA\n");
    fprintf(f, ";Tabla de simbolos\n\n");
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
