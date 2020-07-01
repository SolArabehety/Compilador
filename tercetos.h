#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "tabla_simbolos.h"

/*  Todos los tercetos estan tipificados. Esto va a ser útil para la
    generación de Assembler */
const char* nombreTiposTercetos[11] = { "asignacion", "suma", "resta", "multiplicacion",
                     "division", "comparacion", "salto", "etiqueta", "get", "display", 
                     "desconocido" };

typedef enum tipoTerceto {
    esAsignacion,
    esSuma,
    esResta,
    esMultiplicacion,
    esDivision,
    esComparacion,
    esSalto,
    esEtiqueta,
    esGet,
    esDisplay,
    esDesconocido
} tipoTerceto;

/*  Esta unión significa que elemento podra tener un char*
    o un int (para índices hacio otros tercetos), pero no ambos
    al mismo tiempo */
typedef union unionTerceto {
    char* cad;
    int ind;
} unionTerceto;

/*  Esta struct es necesaria para poder determinar que tipo de 
    valor está siendo almacenado en la unión */
typedef struct elemento {
    /* Los tercetos solo deberían poder tener elementos que sean de tipo
    string, entero (para índices) o nada (indefinido) */
    tipoValor tipo;
    unionTerceto valor;
} elemento;

typedef struct terceto {
    elemento elementos[3];

    /*  Si es real, entero o cadena.
        Esto es un rebusque que hice para que fuera más fácil verificar el tipo
        en expresiones y asignaciones. No aparece en el archivo intermedia.txt y
        no es necesario que todos los tercetos lo tengan definido. */
    tipoValor tipoVal;

    tipoTerceto tipoTerc;
} terceto;

elemento crearElemStr(const char*);
elemento crearElemInt(int);
elemento crearElemNull();
indice crearTerceto(elemento, elemento, elemento, tipoValor, tipoTerceto);
indice cargarConstanteEntera(int);
indice cargarConstanteString(const char*);
indice cargarConstanteReal(float);
indice cargarVariable(const char*, tipoValor);
indice crearTercetoAsignacion(indice, indice);
indice crearTercetoOperacion(const char*, indice, indice);
indice crearTercetoComparacion(indice, indice);
indice crearTercetoBranch(const char*, int);
indice crearTercetoTag();
indice crearTercetoGetValue(const char*);
indice crearTercetoDisplayId(const char*);
indice crearTercetoDisplayCadena(const char*);
indice crearTercetoDisplayReal(float);
indice crearTercetoDisplayEntero(int);
void modificarSaltoTerceto(indice, int);
char* devolverSaltoCondicional(char*);
void imprimirTercetos();
void negarTerceto(int);

/* Índice global para tercetos */
int indTercetos = 0;
/* Array de Tercetos */
terceto tercetos[900];

elemento crearElemStr(const char* str) {
    elemento e;
    e.valor.cad = strdup(str);
    e.tipo = string;
    return e;
}

elemento crearElemInt(int ind) {
    elemento e;
    e.valor.ind = ind;
    e.tipo = entero;
    return e;
}

elemento crearElemNull() {
    elemento e;
    e.tipo = indefinido;
    return e;
}

/*  Crear un terceto con los elementos pasados por parámetro y se Agregamos
    al array global de tercetos. Esta es una función genérica para insertar
    un terceto. Las demás funciones son más específicas y llaman a esta. */
indice crearTerceto(elemento e1, elemento e2, elemento e3, tipoValor tipoV, tipoTerceto tipoT) {
    terceto t;
    indice ind;

    t.elementos[0] = e1;
    t.elementos[1] = e2;
    t.elementos[2] = e3;
    t.tipoVal = tipoV;
    t.tipoTerc = tipoT;

    /* Agregamos el terceto al array global de tercetos */
    tercetos[indTercetos] = t;
    ind.tipo = esTerceto;
    ind.num = indTercetos;
    indTercetos++;

    /* Devolvemos el índice que apunta al terceto creado */
    return ind;
}

/*  Carga una constante entera en la tabla de símbolos, y devuelve una struct
    índice con el valor de la posición en la tabla e indicando que el tipo de
    índice es un símbolo. */
indice cargarConstanteEntera(int val) {
    char buffer[900];
    indice ind;

    sprintf(buffer, "_%d", val);
    borrarChar(buffer, '"');

    ind.tipo = esSimbolo;
    /*  Recordar que, los métodos para insertar en la tabla de símbolos, si el
        símbolo ya está cargado en la tabla, devuelven el índice donde está, por
        lo que no va a crear duplicados */
    ind.num = insertar_ENTERO_en_Tabla(val);

    return ind;
}

indice cargarConstanteString(const char* val) {
    char buffer[900];
    indice ind;

    sprintf(buffer, "_%s", val);
    borrarChar(buffer, '"');

    ind.tipo = esSimbolo;
    ind.num = insertar_STRING_en_Tabla(val);

    return ind;
}

indice cargarConstanteReal(float val) {
    char buffer[900];
    indice ind;

    sprintf(buffer, "_%f", val);
    ind.tipo = esSimbolo;
    ind.num = insertar_REAL_en_Tabla(val);

    return ind;
}

/*  Lo mismo que las funciones para cargar constantes, pero esta es para cargar
    las variables, donde además se debe indicar de que tipo será la variable
    (si es real, entera o string). Se puede usar también para cargar las variables
    de trabajo (las que empiezan con @). */
indice cargarVariable(const char* val, tipoValor tipo) {
    char buffer[900];
    indice ind;

    sprintf(buffer, "_%f", val);
    ind.tipo = esSimbolo;
    ind.num = insertar_ID_en_Tabla(val, tipo);

    return ind;
}

/*  Crear un terceto, donde el primer elemento es una string con el valor
    de alguna operación (ejemplo, "+") y los otros 2 son valores int que
    corresponden a los índices de los tercetos sobre los que se realiza
    la operación. */
indice crearTercetoOperacion(const char* op, indice ind1, indice ind2) {
    elemento elem1, elem2;
    tipoValor tipo1, tipo2, tipoResultado;
    tipoTerceto tipoT;

    if (ind1.tipo == esTerceto) {
        elem1 = crearElemInt(ind1.num);
        tipo1 = tercetos[ind1.num].tipoVal;
    } else { /* El índice es de un símbolo */
        elem1 = crearElemStr(obtenerNombreSimbolo(ind1.num));
        tipo1 = obtenerTipoSimbolo(ind1.num);
    }

    if (ind2.tipo == esTerceto) {
        elem2 = crearElemInt(ind2.num);
        tipo2 = tercetos[ind2.num].tipoVal;
    } else {
        elem2 = crearElemStr(obtenerNombreSimbolo(ind2.num));
        tipo2 = obtenerTipoSimbolo(ind2.num);
    }

    /* Validamos que los tipos de la expresión sean compatibles */
    if (tipo1 == tipo2) {
        tipoResultado = tipo1;
    } else if ((tipo1 == real && tipo2 == entero) || (tipo1 == entero && tipo2 == real)) {
        tipoResultado = real;
    } else {
        printf("\nError Error en la linea %d: La operacion %s entre %s y %s no es compatible.", 
                yylineno, op, nombreTiposVal[tipo1], nombreTiposVal[tipo2]);
        exit(1);
    }
    
    /* Seteamos el tipo de operación */
    switch (op[0]) {
        case '+':
            tipoT = esSuma;
            break;
        case '-':
            tipoT = esResta;
            break;
        case '*':
            tipoT = esMultiplicacion;
            break;
        case '/':
            tipoT = esDivision;
            break;
        default:
            tipoT = esDesconocido;
            break;
    }

    return crearTerceto(crearElemStr(op), elem1, elem2, tipoResultado, tipoT);
}

/*  Similar a crearTercetoOperacion pero con CMP */
indice crearTercetoComparacion(indice ind1, indice ind2) {
    elemento elem1, elem2;
    tipoValor tipo1, tipo2, tipoResultado;
    tipoTerceto tipoT;

    if (ind1.tipo == esTerceto) {
        elem1 = crearElemInt(ind1.num);
        tipo1 = tercetos[ind1.num].tipoVal;
    } else {
        elem1 = crearElemStr(obtenerNombreSimbolo(ind1.num));
        tipo1 = obtenerTipoSimbolo(ind1.num);
    }

    if (ind2.tipo == esTerceto) {
        elem2 = crearElemInt(ind2.num);
        tipo2 = tercetos[ind2.num].tipoVal;
    } else {
        elem2 = crearElemStr(obtenerNombreSimbolo(ind2.num));
        tipo2 = obtenerTipoSimbolo(ind2.num);
    }

    /* Validamos que los tipos de la comparación sean compatibles */
    if (tipo1 == tipo2) {
        tipoResultado = tipo1;
    } else if ((tipo1 == real && tipo2 == entero) || (tipo1 == entero && tipo2 == real)) {
        tipoResultado = real;
    } else {
        printf("\nError Error en la linea %d: La comparacion entre %s y %s no es compatible.", 
                yylineno, nombreTiposVal[tipo1], nombreTiposVal[tipo2]);
        exit(1);
    }

    return crearTerceto(crearElemStr("CMP"), elem1, elem2, tipoResultado, esComparacion);
}

/*
Se coloca la intruccion de leer el dato de la entrada y guardarla en la variable indicada
*/

indice crearTercetoGetValue(const char* val) {
    char buffer[900];
    sprintf(buffer, "%s", val);
    return crearTerceto(crearElemStr("GET"), crearElemStr(buffer), crearElemNull(), indefinido, esGet);
}

indice crearTercetoDisplayId(const char* val) {
    return crearTerceto(crearElemStr("DISPLAY"), crearElemStr(val), crearElemNull(), indefinido, esDisplay);
}

indice crearTercetoDisplayCadena(const char* val) {
    char buffer[900];
    sprintf(buffer, "_%s", val);
    borrarChar(buffer, '"');
    reemplazarChar(buffer, ' ', '_');
    return crearTerceto(crearElemStr("DISPLAY"), crearElemStr(buffer), crearElemNull(), indefinido, esDisplay);
}

indice crearTercetoDisplayReal(float val) {
    char buffer[900];
    sprintf(buffer, "_%f", val);
    return crearTerceto(crearElemStr("DISPLAY"), crearElemStr(buffer), crearElemNull(), indefinido, esDisplay);
}

indice crearTercetoDisplayEntero(int val) {
    char buffer[900];
    sprintf(buffer, "_%d", val);
    return crearTerceto(crearElemStr("DISPLAY"), crearElemStr(buffer), crearElemNull(), indefinido, esDisplay);
}
/*  Crear un terceto, donde el primer y segundo elementos son indices de 
    tercetos. Crea una operación de asignación ("=", ind1, ind2) pero antes
    valida que la asignación pueda realizarse */
indice crearTercetoAsignacion(indice ind1, indice ind2) {
    elemento elem1, elem2;
    tipoValor tipo1, tipo2;

    if (ind1.tipo == esTerceto) {
        elem1 = crearElemInt(ind1.num);
        tipo1 = tercetos[ind1.num].tipoVal;
    } else { /* El índice es de un símbolo */
        elem1 = crearElemStr(obtenerNombreSimbolo(ind1.num));
        tipo1 = obtenerTipoSimbolo(ind1.num);
    }

    if (ind2.tipo == esTerceto) {
        elem2 = crearElemInt(ind2.num);
        tipo2 = tercetos[ind2.num].tipoVal;
    } else { /* El índice es de un símbolo */
        elem2 = crearElemStr(obtenerNombreSimbolo(ind2.num));
        tipo2 = obtenerTipoSimbolo(ind2.num);
    }

    /*  La razón por la cuál se necesitó hacer una función aparte solo para las
        asignaciones es porque la validación de tipo es diferente a la que se hace 
        en crearTercetoOperación. */
    if (tipo1 == tipo2 || (tipo1 == real && tipo2 == entero)) {
        return crearTerceto(crearElemStr("="), elem1, elem2, tipo1, esAsignacion);
    } else {
        printf("\nError en la linea %d: Se intento asignar un %s a una variable de tipo %s.", 
                yylineno, nombreTiposVal[tipo2], nombreTiposVal[tipo1]);
        exit(1);
    }
}

/*  Crear un terceto, donde el primer elemento es el primre valor pasado por 
    parámetro, este debería ser una instrucción de assembler para hacer un 
    salto. El segundo parámetro es el índice del terceto hacia el cuál va a
    hacer el salto. En algunos casos no se conoce el salto al momento de crear
    el branch, en ese caso se debe colocar 0 en el salto y luego se deberá 
    usar la función modificarSaltoTerceto */
indice crearTercetoBranch(const char* op, int salto) {
    return crearTerceto(crearElemStr(op), crearElemInt(salto), crearElemNull(), indefinido, esSalto);
}

/*  Busca al terceto por índice según el primer parámetro de la función,
    y le cambia su segundo valor al entero pasado como segundo parámetro.
    Se debería utilizar para modificar los tercetos que tienen instrucciones
    de Branch. Ojo que si se llama esta función para un índice que apunta a 
    otra cosa que no sea un terceto de branch, puede llegar armar quilombo. */
void modificarSaltoTerceto(indice ind, int salto) {
    tercetos[ind.num].elementos[1].valor.ind = salto;
}

/*  Crea un terceto de etiqueta. La etiqueta va a tener pegada el número de
    terceto asociado. No sé si esto realmente es necesario hacerlo acá ya que
    se podría diferenciar en la generación de assembler por más que diga solo
    TAG, pero en la clase de Mara mostraba que en CGI tenían los números de 
    terceto pegados así que para que quede igual lo modifiqué.
*/
indice crearTercetoTag() {
    char tag[10];

    /*  Nos fijamos si el último terceto creado también es un tag, de ser así
        directamente devolvemos un indice que apunte a tal tag. Esto puede
        pasar cuando hay ifs anidados por ejemplo*/
    if(tercetos[indTercetos - 1].tipoTerc == esEtiqueta) {
        indice ind;
        ind.num = indTercetos - 1;
        ind.tipo = esTerceto;
        return ind;
    }

    sprintf(tag, "TAG%d", indTercetos + 1);
    return crearTerceto(crearElemStr(tag), crearElemNull(), crearElemNull(), indefinido, esEtiqueta);
}

/* Niega la la condicion de un terceto */
void negarTerceto(int numeroTerceto) {
	if(strcmp(tercetos[numeroTerceto].elementos[0].valor.cad, "JNB") == 0) // >=
		 tercetos[numeroTerceto].elementos[0].valor.cad = "JNAE";
	 
	 else if(strcmp(tercetos[numeroTerceto].elementos[0].valor.cad, "JNBE") == 0) // >
		 tercetos[numeroTerceto].elementos[0].valor.cad = "JNA";
	 
	 else if(strcmp(tercetos[numeroTerceto].elementos[0].valor.cad, "JNA") == 0) // <=
		 tercetos[numeroTerceto].elementos[0].valor.cad = "JNBE";
	 
	 else if(strcmp(tercetos[numeroTerceto].elementos[0].valor.cad, "JNAE") == 0) // <
		 tercetos[numeroTerceto].elementos[0].valor.cad = "JNB";
	 
	 else if(strcmp(tercetos[numeroTerceto].elementos[0].valor.cad, "JNE") == 0) // !=
		 tercetos[numeroTerceto].elementos[0].valor.cad = "JE";
	 
	 else if(strcmp(tercetos[numeroTerceto].elementos[0].valor.cad, "JE") == 0) // ==
		 tercetos[numeroTerceto].elementos[0].valor.cad = "JNE";

}

/**	
	Retorna la instruccion assembler correspondiente al caracter recibido
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

void imprimirTercetos() {
    if (DEBUG) printf("Generando archivo intermedia.txt...\n");
    FILE *gci;
    int i, j;

    if ((gci = fopen("intermedia.txt", "w")) == NULL) {
        printf("No se puede crear el archivo de la lista de tercetos.");
        exit(1);
    }

    fprintf(gci, "\n--- LISTA DE TERCETOS ---\n\n");
    for (i = 0; i < indTercetos; i++) {
        terceto t = tercetos[i];
        fprintf(gci, "%d: (", i + 1);

        for (j = 0; j < 3; j++) {
            elemento e = t.elementos[j];

            switch (e.tipo) {
                case string:
                    fprintf(gci, "%s", e.valor.cad);
                    break;
                case entero: /* Si es entero, es un índice y le sumamos 1 al mostrarlo */
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
    fprintf(gci,"\n--- LISTA DE TERCETOS ---\n");
    
    if (fclose(gci) != 0) {
        printf("No se puede CERRAR el archivo de la lista de tercetos.");
        exit(1);
    }
}
